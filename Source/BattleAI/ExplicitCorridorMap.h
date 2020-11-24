// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <vector>
#include <map>

#include "ExplicitCorridorMap.generated.h"

struct ECMNode
{
	int ID;

	// meta data at this node
	FVector2D location;
	float clearance;
	FVector2D nearest_left;
	FVector2D nearest_right;

	// ID('s) of the next node(s) in the graph
	std::vector<int> nextNodes;
};

struct ECMEdge
{
	ECMNode* begin;
	ECMNode* end;
};

struct Retraction
{
	FVector2D location;
	float clearance;
	FVector2D nearest_left;
	FVector2D nearest_right;
};

typedef std::map<int, ECMNode> ECMGraph;
typedef std::vector<ECMNode> Corridor;

/**
 * 
 */
UCLASS()
class BATTLEAI_API UExplicitCorridorMap : public UObject
{
	GENERATED_BODY()

public:
	// bool InitializeECM(XMLData data);
	bool FindPathWithClearance(FVector2D start, FVector2D goal, float clearance, const Corridor& corridor, std::vector<FVector2D>& _outPath) const;
	
private:
	bool FindCorridor(FVector2D start, FVector2D goal, Corridor& _outCorridor) const;
	bool FindRetractionPoint(FVector2D source, Retraction& _outRetraction) const;

	// --------- HELPER METHODS -------------
	bool IsPointInsideConvexPolygon(const FVector2D& point, const std::vector<FVector2D>& cellPoints) const;
	bool GetRetractionEdge(FVector2D source, ECMEdge& _outEdge, bool& _outOnLeftSide) const;
	FVector2D GetClosestPointToLineSegment(const FVector2D& point, const FVector2D& edgeBegin, const FVector2D& edgeEnd) const;

private:
	ECMGraph _graph;
	std::vector<ECMEdge> _edges;
};
