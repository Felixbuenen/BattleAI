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
	float cellSize;

	UPROPERTY()
	bool DivideAndConquerPathfinding;

public:

	virtual void Initialize(class AActor* terrain) override;
	virtual class AGlobalPath* FindPath(const class AFormation* formation, const FVector& goal) const override;

	UFUNCTION(BlueprintImplementableEvent)
	int CalculateClearance(FVector position, FVector extent);

private:
	UPROPERTY()
	class UAStarSolver* solver;
	
	FVector terrainCenter;
	FVector terrainBbox;
};
