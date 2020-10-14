// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPlanner_AStarGrid.h"

#include "Engine/World.h"

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

void UPathPlanner_AStarGrid::Initialize(AActor* terrain)
{
	// REMOVE COMMENTS

	//FVector origin, bbox;
	//terrain->GetActorBounds(true, origin, bbox);
	//
	//origin = terrain->GetActorLocation();
	//
	//int width = (bbox.X * 2.0f) / cellSize;
	//int height = (bbox.Y * 2.0f) / cellSize;
	//
	//solver->Init(width, height, cellSize, origin, DivideAndConquerPathfinding);
	//
	//// calculate clearance
	//const UWorld* world = GetWorld();
	//for (int x = 0; x < width; x++)
	//{
	//	for (int y = 0; y < height; y++)
	//	{
	//		int clearance = CalculateClearance(solver->GetPosition(x + y * width), FVector(cellSize*0.5f));
	//		solver->SetClearance(clearance, x + y * width);
	//	}
	//}
	//
	//terrainBbox = bbox;
	//terrainCenter = origin;
}


AGlobalPath* UPathPlanner_AStarGrid::FindPath(const AFormation* formation, const FVector& goal) const
{
	// REMOVE COMMENTS

	//AGlobalPath* result = GetWorld()->SpawnActor<AGlobalPath>();
	//AFormationCommander* commander = formation->GetCommander();
	//FVector start = commander->GetActorLocation();
	//
	//float boundX = terrainBbox.X;
	//float boundY = terrainBbox.Y;
	//
	//int startIndexX = (int)((start.X - terrainCenter.X + boundX) / cellSize);
	//int startIndexY = (int)((start.Y - terrainCenter.Y + boundY) / cellSize);
	//int goalIndexX = (int)((goal.X - terrainCenter.X + boundX) / cellSize);
	//int goalIndexY = (int)((goal.Y - terrainCenter.Y + boundY) / cellSize);
	//
	//float formWidth, formHeight;
	//formation->GetFormationSize(formWidth, formHeight);
	//FVector2D formationBboxExtent(formWidth * 0.5f, formHeight * 0.5f);
	//
	//std::vector<NodePosition> outPathPoints;
	//bool foundPath = solver->Solve(formationBboxExtent, start.X, start.Y, goal.X, goal.Y, outPathPoints);
	//if (!foundPath)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Could not find path!"));
	//	return nullptr;
	//}
	//
	//// ---- DEBUG ---
	//const UWorld* world = GetWorld();
	//if (world)
	//{
	//	FVector dir = commander->GetActorForwardVector();
	//	FVector2D dir2D;
	//	dir2D.X = dir.X;
	//	dir2D.Y = dir.Y;
	//	dir2D.Normalize();
	//	solver->DrawDivisions(world, formationBboxExtent, dir2D, startIndexX, startIndexY);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Could not find world!"));
	//}
	//// -------------
	//
	//
	//commander->GetCurrentPath()->InitPath(outPathPoints);
	//
	//return commander->GetCurrentPath();

	return nullptr;
}