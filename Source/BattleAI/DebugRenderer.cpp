// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugRenderer.h"

#include "PathPlanner.h"
#include "PathPlanner_AStarGrid.h"
#include "DebugRenderer_AStarGrid.h"

// returns a debug renderer that corresponds to the correct planner
UDebugRenderer* UDebugRenderer::GetNewDebugRenderer(class UPathPlanner* planner)
{
	if(planner->IsA(UPathPlanner_AStarGrid::StaticClass()))
	{
		UDebugRenderer_AStarGrid* gridRenderer = NewObject<UDebugRenderer_AStarGrid>();
		return gridRenderer;
	}

	UE_LOG(LogTemp, Error, TEXT("No debug renderer implementation exists for active path planner!"));
	return nullptr;
}

void UDebugRenderer::Initialize(class UWorld* worldRef, class UPathPlanner* plannerRef)
{
	_plannerRef = plannerRef;
	_worldRef = worldRef;

	SetupRenderDelegates();
}

void UDebugRenderer::Render(int debugKey) const { }

void UDebugRenderer::SetupRenderDelegates() { }

void UDebugRenderer::DrawPaths() const { }