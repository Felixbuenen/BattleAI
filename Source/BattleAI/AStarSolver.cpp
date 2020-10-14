// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarSolver.h"

#include <queue>
#include <limits>
#include <cmath>
#include <functional>
#include <list>
#include <vector>

#include "WorldCollision.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UAStarSolver::UAStarSolver()
{
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

int UAStarSolver::GetIndex(const FVector2D& position) const
{
	float cellExtent = cellSize * 0.5f;

	int x = (position.X - leftEdge - cellExtent) / cellSize;
	int y = (position.Y - upperEdge - cellExtent) / cellSize;

	return (x + y * gridWidth);
}

int UAStarSolver::GetClearance(int index) const
{
	return grid[index]->clearance;
}

void UAStarSolver::Init(const int width, const int height, const int cellSize, const FVector& origin, bool divAndConq)
{
	this->cellSize = cellSize;
	this->gridWidth = width;
	this->gridHeight = height;
	this->divAndConq = divAndConq;
	
	leftEdge = origin.X - (gridWidth * cellSize / 2);
	upperEdge = origin.Y - (gridHeight * cellSize / 2);
	float extent = cellSize / 2.f;
	for (int y = 0; y < gridHeight; y++)
	{
		for (int x = 0; x < gridWidth; x++)
		{
			Node* node = new Node();
			node->x = leftEdge + x * cellSize + extent;
			node->y = upperEdge + y * cellSize + extent;
	
			grid.push_back(node);
		}
	}
	
	maxGridIndex = (int)grid.size() - 1;
	
	// init neighbors
	for (int y = 0; y < gridHeight; y++)
	{
		for (int x = 0; x < gridWidth; x++)
		{
			Node* node = grid[x + y * gridWidth];
	
			if (x > 0 && y > 0) node->neighbors.push_back(grid[x - 1 + (y-1) * gridWidth]);
			if (y > 0) node->neighbors.push_back(grid[x + (y - 1) * gridWidth]);
			if (x < (gridWidth -1) && y > 0) node->neighbors.push_back(grid[x + 1 + (y-1) * gridWidth]);
			if (x > 0) node->neighbors.push_back(grid[x - 1 + y * gridWidth]);
			if (x < (gridWidth - 1)) node->neighbors.push_back(grid[x + 1 + y * gridWidth]);
			if (x > 0 && y < (gridWidth - 1)) node->neighbors.push_back(grid[x-1 + (y + 1) * gridWidth]);
			if (y < (gridWidth - 1)) node->neighbors.push_back(grid[x + (y + 1) * gridWidth]);
			if (x < (gridWidth - 1) && y < (gridWidth - 1)) node->neighbors.push_back(grid[x + 1 + (y + 1) * gridWidth]);
		}
	}
}

// reference used: https://github.com/OneLoneCoder/videos/blob/master/OneLoneCoder_PathFinding_AStar.cpp
bool UAStarSolver::Solve(const FVector2D bboxExtent, const int startX, const int startY, const int goalX, const int goalY, std::vector<NodePosition>& outPath)
{
	int start = startX + startY * gridWidth;
	int goal = goalX + goalY * gridWidth;
	
	//UE_LOG(LogTemp, Error, TEXT("start: %d, goal: %d"), start, goal);
	
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
	//if (grid[start]->clearance * cellSize < bboxExtent.Size())
	//if (TestClearance(bboxExtent.Size(), grid[start]->clearance, bboxExtent, FVector2D(grid[start]->x, grid[start]->y), FVector2D::ZeroVector))
	//{
	//	UE_LOG(LogTemp, Error, TEXT("not enough clearance for formation (%f needed, %f at start location)!"), bboxExtent.Size(), grid[start]->clearance);
	//	return false;
	//}
	
	// reset all nodes in the grid
	for (Node* node : grid)
	{
		node->globalGoal = 0xffffffff;
		node->localGoal = 0xffffffff;
		node->parent = nullptr;
		node->processed = false;
	}
	
	float formationRadius = bboxExtent.Size();
	//UE_LOG(LogTemp, Error, TEXT("BBOX WIDTH: %f, HEIGHT: %f"), bboxExtent.Y, bboxExtent.X);
	
	Node* goalNode = grid[goal];
	//UE_LOG(LogTemp, Warning, TEXT("goal node pos = (%f, %f)"), goalNode->x, goalNode->y);
	
	std::list<Node*> openQueue;
	
	Node* startNode = grid[start];
	Node* current = startNode;
	current->localGoal = 0.f;
	current->globalGoal = Heuristic(current, goalNode);
	openQueue.push_back(current);
	
	//UE_LOG(LogTemp, Warning, TEXT("cell size is %f"), cellSize);
	//UE_LOG(LogTemp, Warning, TEXT("formation radius is %f"), formationRadius);
	
	int nodeCounter = 0;
	
	while (!openQueue.empty() && current != goalNode)
	{
		// Sort Untested nodes by global goal, so lowest is first
		openQueue.sort([](const Node* lhs, const Node* rhs) { return lhs->globalGoal < rhs->globalGoal; });
	
		// make sure we don't process any nodes we've already visited
		while (!openQueue.empty() && openQueue.front()->processed)
		{
			openQueue.pop_front();
		}
	
		if (openQueue.empty()) break;
	
		current = openQueue.front();
		current->processed = true;
		FVector2D currentDirection = FVector2D(0, 0);
		if (current->parent != nullptr)
		{
			currentDirection = FVector2D(current->parent->x - current->x, current->parent->y - current->y);
			currentDirection.Normalize();
		}
		
		// process neighbors
		for (Node* neighbor : current->neighbors)
		{
			if (!neighbor->processed && TestClearance(formationRadius, neighbor->clearance, bboxExtent, FVector2D(neighbor->x, neighbor->y), currentDirection))
				openQueue.push_back(neighbor);
	
			float potentialNewLocal = (current->localGoal + distance(current, goalNode));
			
			FVector2D newDirection = FVector2D(current->x - neighbor->x, current->y - neighbor->y);
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
	
			nodeCounter++;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("NODES CONSIDERED: %d"), nodeCounter);
	
	outPath = CreatePath(startNode, goalNode);
	if (outPath.empty())
	{
		UE_LOG(LogTemp, Error, TEXT("could not find path"));
		return false;
	}
	
	return true;
}

void UAStarSolver::DrawDivisions(const UWorld* world, const FVector2D bboxExtent, const FVector2D dir, const int startX, const int startY) const
{
	// DEBUG

	int start = startX + startY * gridWidth;

	TestClearance(bboxExtent.Size(), grid[start]->clearance, bboxExtent, FVector2D(grid[start]->x, grid[start]->y), dir, 0, world);
}


bool UAStarSolver::TestClearance(const float formRadius, const float clearance, const FVector2D& extent, const FVector2D position, const FVector2D& dir, int depth, const UWorld* world) const
{
	if (!divAndConq) return clearance * cellSize > formRadius;
	
	if (clearance * cellSize > formRadius && !world)
	{
		return true;
	}
	
	if (depth == maxDepth) return false;
	
	//UE_LOG(LogTemp, Error, TEXT("Going into recursion with depth %f"), (float)depth);
	
	// ----- DEBUG -----
	if (world)
	{
		float colScal = (float)depth / 2.f;
		FColor col = FColor::MakeRedToGreenColorFromScalar(colScal);
	
		DrawDebugSphere(world, FVector(position.X, position.Y, 60.f), formRadius, 20, col, true, 20);
	}
	// --------------
	
	int outIndexL, outIndexR;
	float outClearanceL, outClearanceR;
	FVector2D outExtent, outPosL, outPosR;
	
	if (!DivideFormation(extent, position, dir, outExtent, outPosL, outPosR, outIndexL, outIndexR, outClearanceL, outClearanceR))
	{
		return false;
	}
	float newFormClearance = outExtent.Size();
	
	UE_LOG(LogTemp, Error, TEXT("Going into left recursion with depth %d"), depth);
	bool leftIsClear = TestClearance(newFormClearance, outClearanceL, outExtent, outPosL, dir, depth + 1, world);
	UE_LOG(LogTemp, Error, TEXT("Going into right recursion with depth %d"), depth);
	bool rightIsClear = TestClearance(newFormClearance, outClearanceR, outExtent, outPosR, dir, depth + 1, world);
	
	return leftIsClear && rightIsClear;
}

// assume that direction is normalized!
bool UAStarSolver::DivideFormation(const FVector2D& extent, const FVector2D position, const FVector2D& dir, FVector2D& outExtent, FVector2D& outPosLeft, FVector2D& outPosRight, int& outIndexLeft, int& outIndexRight, float& outClearanceLeft, float& outClearanceRight) const
{
	int longestAxis;
	FVector2D offsetDir;
	
	if (extent.X > extent.Y)
	{
		longestAxis = 0;
		offsetDir = FVector2D(dir.Y, -dir.X);
	}
	else
	{
		longestAxis = 1;
		offsetDir = dir;
	}
	
	float offset = extent[longestAxis] * 0.5f;
	
	outExtent = extent;
	outExtent[longestAxis] *= 0.5f;
	
	// get the mid points of the 2 split-up bboxes in world space
	outPosRight = position + offsetDir * offset;
	outPosLeft = position - offsetDir * offset;
	
	outIndexRight = GetIndex(outPosRight);
	outIndexLeft = GetIndex(outPosLeft);
	
	if (outIndexLeft < 0 || outIndexRight < 0 || outIndexLeft > maxGridIndex || outIndexRight > maxGridIndex) return false;
	
	// TODO: adjust clearance size to incorporate discretization error of grid cell
	outClearanceRight = grid[outIndexRight]->clearance + SQRT_TWO;
	outClearanceLeft = grid[outIndexLeft]->clearance + SQRT_TWO;
	
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
	float xDiff = to->x - from->x;
	float yDiff = to->y - from->y;
	float length2 = xDiff * xDiff + yDiff * yDiff;

	return (int)(sqrt(length2));
}

int UAStarSolver::Heuristic(const Node* from, const Node* to) const
{
	return distance(from, to);
}
