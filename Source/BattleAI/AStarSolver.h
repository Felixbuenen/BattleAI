// A C++ Program to implement A* Search Algorithm 

#pragma once


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

enum Direction
{
	NW,
	N,
	NE,
	W,
	C,
	E,
	SW,
	S,
	SE
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
	
	void Init(const int width, const int height, const int cellSize, const AActor* gridObject);
	bool Solve(const FVector2D bboxExtent, const int start, const int goal, std::vector<NodePosition>& outPath);
	void SetClearance(int clearance, int index);

	FORCEINLINE FVector GetPosition(int index) const;
	FORCEINLINE int GetClearance(int index) const;
	FORCEINLINE std::vector<Node*> GetGrid() const { return grid; }

private:

	FORCEINLINE int Heuristic(const Node* from, const Node* to) const;
	FORCEINLINE int distance(const Node* from, const Node* to) const;
	std::vector<NodePosition> CreatePath(Node* start, Node* goal);
	std::vector<Node*> grid;
	Direction GetCurrentDirection(float fromX, float toX, float fromY, float toY) const; // replace by lookup

	class USphereComponent* sphereCollider;
	float cellSize;
};
