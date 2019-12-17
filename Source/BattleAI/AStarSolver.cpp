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
	//sphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//sphereCollider->SetCollisionResponseToChannel(ECollisionChannel::, ECollisionResponse::ECR_Overlap);
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
	if (start == goal) return false;
	if (start < 0 || start >= 100*99) return false;
	if (goal < 0 || goal >= 100*99) return false;
	// reset all nodes in the grid
	for (Node* node : grid)
	{
		node->globalGoal = 0xffffffff;
		node->localGoal = 0xffffffff;
		node->parent = nullptr;
		node->processed = false;
	}

	float formationRadius = bboxExtent.Size();

	Node* goalNode = grid[goal];

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

		// process neighbors
		for (Node* neighbor : current->neighbors)
		{
			if (!neighbor->processed && neighbor->clearance * cellSize > formationRadius)
				openQueue.push_back(neighbor);

			float potentialNewLocal = (current->localGoal + distance(current, neighbor));

			if (potentialNewLocal < neighbor->localGoal)
			{
				neighbor->parent = current;
				neighbor->localGoal = potentialNewLocal;
				neighbor->globalGoal = neighbor->localGoal + Heuristic(neighbor, goalNode);
			}

		}
	}
	outPath = CreatePath(startNode, goalNode);
	if (outPath.empty()) return false;

	return true;

}

std::vector<NodePosition> UAStarSolver::CreatePath(Node* start, Node* goal)
{
	std::vector<NodePosition> path;
	if (goal->parent == nullptr) return path;

	NodePosition pos;
	pos.x = goal->x;
	pos.y = goal->y;
	path.push_back(pos);
	Node* p = goal->parent;
	while (p != nullptr)
	{
		NodePosition newPos;
		newPos.x = p->x;
		newPos.y = p->y;

		path.push_back(newPos);
		p = p->parent;
	}

	return path;
}

// TWO SAME FUNCTIONS...
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