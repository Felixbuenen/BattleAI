// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPlanner.h"

#include "GlobalPath.h"
#include "Formation.h"
#include "DebugRenderer.h"

void UPathPlanner::Initialize(class AActor* terrain, const TSubclassOf<AActor>& obstacle)
{
	_debugRenderer = UDebugRenderer::GetNewDebugRenderer(this);
	if (_debugRenderer == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDebugRenderer could not be created. abort..."));
		return;
	}

	// use the terrain object to get a reference to the world
	UWorld* world = terrain->GetWorld();
	if (world == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWorld could not be created. abort..."));
		return;
	}
	_debugRenderer->Initialize(world, this);
}

AGlobalPath* UPathPlanner::FindPath(const AFormation* formation, const FVector& goal) const
{
	return nullptr;
}

void UPathPlanner::DrawDebugInfo(int infoKey) const
{
	if (!IsValid(_debugRenderer)) return;

	_debugRenderer->Render(infoKey);
}