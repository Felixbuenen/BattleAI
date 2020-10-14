// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAILevel.h"

#include "Engine/StaticMeshActor.h"
#include "DrawDebugHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "Components/SphereComponent.h"

#include "Formation.h"
#include "AStarSolver.h"
#include "GlobalPath.h"
#include "BattleAIGameState.h"
#include "FormationCommander.h"
#include "PathPlanner.h"
#include "PathPlanner_AStarGrid.h"

ABattleAILevel::ABattleAILevel()
{
	PrimaryActorTick.bCanEverTick = true;

	// TODO: create array of all pathfinders
}

void ABattleAILevel::InitPathfindingInfo()
{
	// init pathfinder
	pathfinder = NewObject<UPathPlanner_AStarGrid>(this);
	pathfinder->Initialize(floor, obstacle);
}

// TODO: do debug drawing in path planner class
void ABattleAILevel::Tick(float DeltaTime)
{
	//bool drawSDF = ((ABattleAIGameState*)(GetWorld()->GetGameState()))->ShowSDF;
	//
	//if (drawSDF)
	//{
	//	const UWorld* world = GetWorld();
	//
	//	for (int x = 0; x < gridDimension; x++)
	//	{
	//		for (int y = 0; y < gridDimension; y++)
	//		{
	//			//FVector center(x * CellExtent * 2 - levelBoundX + CellExtent, y * CellExtent * 2 - levelBoundY + CellExtent, 70);
	//			FVector center = pathfinder->GetPosition(x + y * gridDimension);
	//			center.Z = 70.f;
	//			FVector extend(CellExtent);
	//
	//			int clearance = pathfinder->GetClearance(x + y * gridDimension);
	//
	//			const int maxSearchDepth = 20;
	//			FColor clr = FColor::MakeRedToGreenColorFromScalar(clearance / (float)(maxSearchDepth));
	//
	//			if (clearance == 0)
	//			{
	//				DrawDebugBox(world, center, extend, FColor::Red, false);
	//			}
	//			else
	//			{
	//				DrawDebugBox(world, center, extend, clr, false);
	//			}
	//		}
	//	}
	//}
}

UPathPlanner* ABattleAILevel::GetPathPlanner() const
{
	return pathfinder;
}
