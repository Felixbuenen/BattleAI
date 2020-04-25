// A C++ Program to implement A* Search Algorithm 

#pragma once

#define SQRT_TWO 1.41421356

#include <vector>

// TODO: optimize data packing
struct Node 
{
	int x, y; // x, y world coordinates
	unsigned int localGoal;
	unsigned int globalGoal;
	int clearance;
	bool processed;

	std::vector<Node*> neighbors;
	Node* parent;

	bool operator<(const Node& n2)
	{
		return globalGoal < n2.globalGoal;
	}
};

struct NodePosition
{
	int x, y;
};

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AStarSolver.generated.h" 

/**
 * 
 */
UCLASS()
class BATTLEAI_API UAStarSolver : public UObject
{
	GENERATED_BODY()

public:
	UAStarSolver();
	~UAStarSolver();
	
	void Init(const int width, const int height, const int cellSize, const AActor* gridObject, bool divAndConq);
	bool Solve(const FVector2D bboxExtent, const int startX, const int startY, const int goalX, const int goalY, std::vector<NodePosition>& outPath);
	void SetClearance(int clearance, int index);

	FORCEINLINE FVector GetPosition(int index) const;
	FORCEINLINE int GetClearance(int index) const;
	FORCEINLINE std::vector<Node*> GetGrid() const { return grid; }

	void DrawDivisions(const UWorld* world, const FVector2D bboxExtent, const FVector2D dir, const int startX, const int startY) const;

private:

	FORCEINLINE int Heuristic(const Node* from, const Node* to) const;
	FORCEINLINE int distance(const Node* from, const Node* to) const;
	FORCEINLINE int GetIndex(const FVector2D& position) const;

	FORCEINLINE bool TestClearance(const float formRadius, const float clearance, const FVector2D& extent, const FVector2D position, const FVector2D& dir, int depth = 0, const UWorld* world = nullptr) const;
	bool DivideFormation(const FVector2D& extent, const FVector2D position, const FVector2D& dir, FVector2D& outExtent, FVector2D& outPosLeft, FVector2D& outPosRight, int& outIndexLeft, int& outIndexRight, float& outClearanceLeft, float& outClearanceRight) const;
	std::vector<NodePosition> CreatePath(Node* start, Node* goal);
	std::vector<Node*> grid;

	bool divAndConq;

	int gridDimension;
	float cellSize;
	int leftEdge;
	int upperEdge;
	int maxGridIndex;
	const int maxDepth = 3; // TODO: make define
};
