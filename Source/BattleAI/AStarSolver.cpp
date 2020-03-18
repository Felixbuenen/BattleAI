// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarSolver.h"

#include <queue>
#include <limits>
#include <cmath>
#include <functional>
#include <list>

#include "WorldCollision.h"
#include "Components/SphereComponent.h"

UAStarSolver::UAStarSolver()
{
	sphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("sphere collider"));
	sphereCollider->SetSphereRadius(500.f);
}

UAStarSolver::~UAStarSolver()
{
	for (auto node : grid)
	{
		delete node;
	}
}

void UAStarSolver::SetClearance(int clearance, int index)
{
	grid[index]->clearance = clearance;
}

FVector UAStarSolver::GetPosition(int index) const
{
	return FVector(grid[index]->x, grid[index]->y, 0.f);
}

int UAStarSolver::GetClearance(int index) const
{
	return grid[index]->clearance;
}

void UAStarSolver::Init(const int width, const int height, const int cellSize, const AActor* gridObject)
{
	this->cellSize = cellSize;

	FVector origin = gridObject->GetActorLocation();
	int leftEdge = origin.X - (width * cellSize / 2);
	int upperEdge = origin.Y - (height * cellSize / 2);
	float extent = cellSize / 2.f;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Node* node = new Node();
			node->x = leftEdge + x * cellSize + extent;
			node->y = upperEdge + y * cellSize + extent;

			grid.push_back(node);
		}
	}

	// init neighbors
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			Node* node = grid[x + y * width];

			if (x > 0 && y > 0) node->neighbors.push_back(grid[x - 1 + (y-1) * width]);
			if (y > 0) node->neighbors.push_back(grid[x + (y - 1) * width]);
			if (x < (width-1) && y > 0) node->neighbors.push_back(grid[x + 1 + (y-1) * width]);
			if (x > 0) node->neighbors.push_back(grid[x - 1 + y * width]);
			if (x < (width - 1)) node->neighbors.push_back(grid[x + 1 + y * width]);
			if (x > 0 && y < (height - 1)) node->neighbors.push_back(grid[x-1 + (y + 1) * width]);
			if (y < (height - 1)) node->neighbors.push_back(grid[x + (y + 1) * width]);
			if (x < (width - 1) && y < (height - 1)) node->neighbors.push_back(grid[x + 1 + (y + 1) * width]);
		}
	}
}

// reference used: https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_PathFinding_AStar.cpp
bool UAStarSolver::Solve(const FVector2D bboxExtent, const int start, const int goal, std::vector<NodePosition>& outPath)
{
	UE_LOG(LogTemp, Error, TEXT("start: %d, goal: %d"), start, goal);

	if (start == goal) {
		UE_LOG(LogTemp, Error, TEXT("could not find path: START==GOAL"));
		return false;
	}
	if (start < 0 || start >= (int)grid.size()) {
		UE_LOG(LogTemp, Error, TEXT("could not find path: INVALID START POSITION (%d). Grid size is %d"), start, (int)grid.size());
		return false;
	}
	if (goal < 0 || goal >= (int)grid.size()) {
		UE_LOG(LogTemp, Error, TEXT("could not find path: INVALID GOAL POSITION"));
		return false;
	}
	if (grid[start]->clearance * cellSize < bboxExtent.Size())
	{
		UE_LOG(LogTemp, Error, TEXT("not enough clearance for formation (%f needed, %f at start location)!"), bboxExtent.Size(), grid[start]->clearance);
		return false;
	}

	// reset all nodes in the grid
	for (Node* node : grid)
	{
		node->globalGoal = 0xffffffff;
		node->localGoal = 0xffffffff;
		node->parent = nullptr;
		node->processed = false;
	}

	float formationRadius = bboxExtent.Size();
	UE_LOG(LogTemp, Error, TEXT("BBOX WIDTH: %f, HEIGHT: %f"), bboxExtent.Y, bboxExtent.X);


	Node* goalNode = grid[goal];
	UE_LOG(LogTemp, Warning, TEXT("goal node pos = (%f, %f)"), goalNode->x, goalNode->y);

	//std::priority_queue<Node*, std::vector<Node*>, std::less<Node*>> openQueue;
	std::list<Node*> openQueue;

	Node* startNode = grid[start];
	startNode->localGoal = 0.f;
	startNode->globalGoal = Heuristic(startNode, goalNode);
	openQueue.push_back(startNode);

	UE_LOG(LogTemp, Warning, TEXT("cell size is %f"), cellSize);
	UE_LOG(LogTemp, Warning, TEXT("formation radius is %f"), formationRadius);

	while (!openQueue.empty())
	{
		// Sort Untested nodes by global goal, so lowest is first
		openQueue.sort([](const Node* lhs, const Node* rhs) { return lhs->globalGoal < rhs->globalGoal; });

		// make sure we don't process any nodes we've already visited
		while (!openQueue.empty() && openQueue.front()->processed)
		{
			openQueue.pop_front();
		}

		if (openQueue.empty()) break;

		Node* current = openQueue.front();
		current->processed = true;
		Direction currentDirection = current->parent != nullptr ? GetCurrentDirection(current->parent->x, current->x, current->parent->y, current->y) : Direction::C;
		
		// process neighbors
		for (Node* neighbor : current->neighbors)
		{
			if (!neighbor->processed && neighbor->clearance * cellSize > formationRadius)
				openQueue.push_back(neighbor);

			float potentialNewLocal = (current->localGoal + distance(current, neighbor));
			
			Direction newDirection = GetCurrentDirection(current->x, neighbor->x, current->y, neighbor->y);
			if (newDirection != currentDirection)
			{
				potentialNewLocal += 250.f; // add penalty if agent needs to change direction (prevent zigzagging)
			}

			if (potentialNewLocal < neighbor->localGoal)
			{
				neighbor->parent = current;
				neighbor->localGoal = potentialNewLocal;
				neighbor->globalGoal = neighbor->localGoal + Heuristic(neighbor, goalNode);
			}

		}
	}
	outPath = CreatePath(startNode, goalNode);
	if (outPath.empty())
	{
		UE_LOG(LogTemp, Error, TEXT("could not find path"));
		return false;
	}

	return true;
}

std::vector<NodePosition> UAStarSolver::CreatePath(Node* start, Node* goal)
{
	std::vector<NodePosition> path;
	if (goal->parent == nullptr) return path;

	// add first node to list
	NodePosition prevPos;
	NodePosition pos;
	pos.x = goal->x;
	pos.y = goal->y;
	path.push_back(pos);
	
	prevPos = pos;
	Node* p = goal->parent;
	Direction currentDir = Direction::C;
	NodePosition newPos;

	while (p != nullptr)
	{
		// TODO: deze code werkt niet met UE4 splines (automatic tangents)
		//Direction newDir = GetCurrentDirection(prevPos.x, p->x, prevPos.y, p->y);
		//if(newDir == currentDir)
		//{
		//	p = p->parent;
		//	continue;
		//}

		//currentDir = newDir;
		newPos.x = p->x;
		newPos.y = p->y;
		prevPos = newPos;

		path.push_back(newPos);
		p = p->parent;
	}

	NodePosition startNode;
	startNode.x = start->x;
	startNode.y = start->y;
	// check if the last added node was the start node
	if (startNode.x != newPos.x && startNode.y != newPos.y)
	{
		path.push_back(startNode);
	}

	return path;
}

int UAStarSolver::distance(const Node* from, const Node* to) const
{
	float xDiff = abs(to->x - from->x);
	float yDiff = abs(to->y - from->y);
	float length2 = xDiff * xDiff + yDiff * yDiff;

	return (int)(sqrt(length2));
}

int UAStarSolver::Heuristic(const Node* from, const Node* to) const
{
	return distance(from, to);
}

Direction UAStarSolver::GetCurrentDirection(float fromX, float toX, float fromY, float toY) const
{

	if (fromX == toX)
	{
		if (fromY == toY) { return Direction::C; }
		else if (toY > fromY) { return Direction::N; }
		else { return Direction::S; }
	}

	else if (toX > fromX)
	{
		if (toY == fromY) { return Direction::E; }
		else if (toY > fromY) { return Direction::NE; }
		else { return Direction::SE; }
	}

	else
	{
		if (toY == fromY) { return Direction::W; }
		else if (toY > fromY) { return Direction::NW; }
		else { return Direction::SW; }
	}
}