// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPlanner_AStarGrid.h"

#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

#include "AStarSolver.h"
#include "Formation.h"
#include "FormationCommander.h"
#include "GlobalPath.h"

#include <vector>

UPathPlanner_AStarGrid::UPathPlanner_AStarGrid()
{
	// TODO: provide some environmental information to set up datastructures
	// create solver, init solver
}

void UPathPlanner_AStarGrid::Initialize(class AActor* terrain, const TSubclassOf<AActor>& obstacle)
{
	Super::Initialize(terrain, obstacle);
	
	FVector origin, bbox;
	terrain->GetActorBounds(true, origin, bbox);
	
	origin = terrain->GetActorLocation();
	
	int width = (bbox.X * 2.0f) / cellSize;
	int height = (bbox.Y * 2.0f) / cellSize;
	
	solver = NewObject<UAStarSolver>();
	solver->Init(width, height, cellSize, origin, DivideAndConquerPathfinding);
	CreateClearanceGrid(width, height, obstacle);
	
	terrainBbox = bbox;
	terrainCenter = origin;
}


AGlobalPath* UPathPlanner_AStarGrid::FindPath(const AFormation* formation, const FVector& goal) const
{
	AGlobalPath* result = GetWorld()->SpawnActor<AGlobalPath>();
	AFormationCommander* commander = formation->GetCommander();
	FVector start = commander->GetActorLocation();
	
	float boundX = terrainBbox.X;
	float boundY = terrainBbox.Y;
	
	int startIndexX = (int)((start.X - terrainCenter.X + boundX) / cellSize);
	int startIndexY = (int)((start.Y - terrainCenter.Y + boundY) / cellSize);
	int goalIndexX = (int)((goal.X - terrainCenter.X + boundX) / cellSize);
	int goalIndexY = (int)((goal.Y - terrainCenter.Y + boundY) / cellSize);
	
	float formWidth, formHeight;
	formation->GetFormationSize(formWidth, formHeight);
	FVector2D formationBboxExtent(formWidth * 0.5f, formHeight * 0.5f);
	
	std::vector<NodePosition> outPathPoints;
	bool foundPath = solver->Solve(formationBboxExtent, startIndexX, startIndexY, goalIndexX, goalIndexY, outPathPoints);
	if (!foundPath)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find path!"));
		return nullptr;
	}
	
	// ---- DEBUG ---
	const UWorld* world = GetWorld();
	if (world)
	{
		FVector dir = commander->GetActorForwardVector();
		FVector2D dir2D;
		dir2D.X = dir.X;
		dir2D.Y = dir.Y;
		dir2D.Normalize();
		solver->DrawDivisions(world, formationBboxExtent, dir2D, startIndexX, startIndexY);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find world!"));
	}
	// -------------
	
	
	commander->GetCurrentPath()->InitPath(outPathPoints);
	
	return commander->GetCurrentPath();
}

void UPathPlanner_AStarGrid::CreateClearanceGrid(int width, int height, const TSubclassOf<AActor>& obstacle)
{
	// calculate clearance
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int clearance = CalculateClearance(solver->GetPosition(x + y * width), FVector(cellSize * 0.5f), obstacle);
			solver->SetClearance(clearance, x + y * width);
		}
	}
}

int UPathPlanner_AStarGrid::CalculateClearance(FVector position, FVector extent, const TSubclassOf<AActor>& obstacle)
{
	FVector boxPosition = position;
	boxPosition.Z += 200.0f;
	FVector boxExtent = extent;
	FVector baseExtent = extent;
	int clearance = -1;
	const int maxSearchDepth = 20;
	bool foundCollision = false;

	TArray<TEnumAsByte<EObjectTypeQuery>> overlapFilter;
	overlapFilter.Emplace(ECC_GameTraceChannel2);
	UWorld* world = GetWorld();
	while (!foundCollision && clearance < maxSearchDepth)
	{
		// check if a collision occurs with the obstacle space
		TArray<AActor*> out;
		foundCollision = UKismetSystemLibrary::BoxOverlapActors(world, boxPosition, boxExtent, TArray<TEnumAsByte<EObjectTypeQuery>>(), obstacle.Get(), TArray<AActor*>(), out);

		clearance++;

		FVector extentIncrease = baseExtent * (clearance + 1) * 2;
		boxExtent = baseExtent + extentIncrease;
	}

	return clearance;
}
