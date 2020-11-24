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
	for (int key : drawDebugKeys)
	{
		DrawPlannerInfo(key);
	}
}

UPathPlanner* ABattleAILevel::GetPathPlanner() const
{
	return pathfinder;
}

void ABattleAILevel::DrawPlannerInfo(int infoKey) const
{
	pathfinder->DrawDebugInfo(infoKey);
}

void ABattleAILevel::ToggleRenderDebugItem(int item)
{
	UE_LOG(LogTemp, Warning, TEXT("lasdjflksdjfjsalkdfj"));
	if (drawDebugKeys.Contains(item))
	{
		UE_LOG(LogTemp, Warning, TEXT("don't draw %d anymore"), item);
		drawDebugKeys.Remove(item);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("draw %d"), item);
		drawDebugKeys.Add(item);
	}
}
