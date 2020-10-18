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
}

UPathPlanner* ABattleAILevel::GetPathPlanner() const
{
	return pathfinder;
}

void ABattleAILevel::ToggleDrawPath(AGlobalPath* path) const
{

}

void ABattleAILevel::ToggleDrawPlannerInfo(int infoKey) const
{
	pathfinder->ToggleDrawDebugInfo(infoKey);
}
