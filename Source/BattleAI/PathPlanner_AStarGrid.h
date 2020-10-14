// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathPlanner.h"
#include "PathPlanner_AStarGrid.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAI_API UPathPlanner_AStarGrid : public UPathPlanner
{
	GENERATED_BODY()
	
public:
	UPathPlanner_AStarGrid();

	UPROPERTY()
	float cellSize = 100.0f; // TODO: this should be visible on level blueprint

	UPROPERTY()
	bool DivideAndConquerPathfinding;

public:

	virtual void Initialize(class AActor* terrain, const TSubclassOf<AActor>& obstacle) override;
	virtual class AGlobalPath* FindPath(const class AFormation* formation, const FVector& goal) const override;

private:
	void CreateClearanceGrid(int width, int height, const TSubclassOf<AActor>& obstacle);
	int CalculateClearance(FVector position, FVector extent, const TSubclassOf<AActor>& obstacle);

private:
	UPROPERTY()
	class UAStarSolver* solver;
	
	FVector terrainCenter;
	FVector terrainBbox;
};
