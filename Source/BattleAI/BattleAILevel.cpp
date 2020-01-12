// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAILevel.h"

#include "Engine/StaticMeshActor.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "AStarSolver.h"
#include "GlobalPath.h"
#include "BattleAIGameState.h"

ABattleAILevel::ABattleAILevel()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABattleAILevel::InitPathfindingInfo()
{
	// init pathfinder
	pathfinder = NewObject<UAStarSolver>();

	FVector origin;
	levelBoundX = floor->GetStaticMeshComponent()->Bounds.BoxExtent.X;
	levelBoundY = floor->GetStaticMeshComponent()->Bounds.BoxExtent.Y;
	origin = floor->GetActorLocation();

	CellExtent = levelBoundX / 100.f;

	pathfinder->Init(100, 100, CellExtent * 2, floor);
	AGlobalPath::SetCellSize(CellExtent * 2);
	
	// calculate clearance
	const UWorld* world = GetWorld();
	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			int clearance = CalculateClearance(pathfinder->GetPosition(x + y * 100), FVector(CellExtent));
			pathfinder->SetClearance(clearance, x + y * 100);
		}
	}
}

void ABattleAILevel::Tick(float DeltaTime)
{
	bool drawSDF = ((ABattleAIGameState*)(GetWorld()->GetGameState()))->ShowSDF;

	if (drawSDF)
	{
		const UWorld* world = GetWorld();

		for (int x = 0; x < 100; x++)
		{
			for (int y = 0; y < 100; y++)
			{
				//FVector center(x * CellExtent * 2 - levelBoundX + CellExtent, y * CellExtent * 2 - levelBoundY + CellExtent, 70);
				FVector center = pathfinder->GetPosition(x + y * 100);
				center.Z = 70.f;
				FVector extend(CellExtent);

				int clearance = pathfinder->GetClearance(x + y * 100);

				const int maxSearchDepth = 20;
				FColor clr = FColor::MakeRedToGreenColorFromScalar(clearance / (float)(maxSearchDepth));

				if (clearance == 0)
				{
					DrawDebugBox(world, center, extend, FColor::Red, false);
				}
				else
				{
					DrawDebugBox(world, center, extend, clr, false);
				}
			}
		}
	}
}

AGlobalPath* ABattleAILevel::FindGlobalPath(AGlobalPath* pathRef, FVector commanderLocation, FVector destination)
{
	FVector gridCenter = floor->GetActorLocation();

	float boundX = floor->GetStaticMeshComponent()->Bounds.BoxExtent.X;
	float boundY = floor->GetStaticMeshComponent()->Bounds.BoxExtent.Y;
	float cellSize= boundX / 50.f;

	int startIndexX = (int)((commanderLocation.X - gridCenter.X + boundX) / cellSize);
	int startIndexY = (int)((commanderLocation.Y - gridCenter.Y + boundY) / cellSize);
	int goalIndexX = (int)((destination.X - gridCenter.X + boundX) / cellSize);
	int goalIndexY = (int)((destination.Y - gridCenter.Y + boundY) / cellSize);


	std::vector<NodePosition> path;
	FVector2D formationBboxExtent(300, 100); // hard coded for now
	bool foundPath = pathfinder->Solve(formationBboxExtent, startIndexX + startIndexY * 100, goalIndexX + goalIndexY * 100, path);
	UWorld* world = GetWorld();

	if (!foundPath)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find path!"));
		return nullptr;
	}

	pathRef->InitPath(path);

	//for (NodePosition pos : path)
	//{
	//	FVector center(pos.x, pos.y, 70);
	//	FVector extend(cellSize / 2.f);
	//	DrawDebugSolidBox(world, center, extend, FColor::Red, false, 100.f);
	//}

	return pathRef;
}