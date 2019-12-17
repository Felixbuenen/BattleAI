// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAILevel.h"

#include "Engine/StaticMeshActor.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "AStarSolver.h"
#include "GlobalPath.h"

ABattleAILevel::ABattleAILevel()
{
}

void ABattleAILevel::InitPathfindingInfo()
{
	// init pathfinder
	pathfinder = NewObject<UAStarSolver>();

	FVector origin;
	float boundX = floor->GetStaticMeshComponent()->Bounds.BoxExtent.X;
	float boundY = floor->GetStaticMeshComponent()->Bounds.BoxExtent.Y;
	origin = floor->GetActorLocation();

	float cellExtent = boundX / 100.f;

	pathfinder->Init(100, 100, cellExtent * 2, floor);


	const UWorld* world = GetWorld();

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			FVector center(x * cellExtent*2 - boundX + cellExtent, y * cellExtent*2 - boundY + cellExtent, 70);
			FVector extend(cellExtent);

			int clearance = GetClearance(pathfinder->GetPosition(x + y * 100), FVector(cellExtent));
			pathfinder->SetClearance(clearance, x + y * 100);

			// debug drawing. Note: hard-coded max search depth value
			const int maxSearchDepth = 20;
			FColor clr = FColor::MakeRedToGreenColorFromScalar(clearance / (float)(maxSearchDepth));

			if (clearance == 0)
			{
				DrawDebugBox(world, center, extend, FColor::Red, false, 100.f);
			}
			else
			{
				DrawDebugBox(world, center, extend, clr, false, 100.f);
			}
			// end debug drawing
		}
	}


}

AGlobalPath* ABattleAILevel::FindGlobalPath(FVector commanderLocation, FVector destination)
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

	if (foundPath)
	{
		// spawn global path object
		if (currentPath)
		{
			world->DestroyActor(currentPath);
		}

		//currentPath = GetWorld()->SpawnActor<AGlobalPath>(TSubclassOf<AGlobalPath>(), FName("Global Path"), FVector(0.f), FRotator());
		currentPath = GetWorld()->SpawnActor<AGlobalPath>(AGlobalPath::StaticClass(), FVector(0.f), FRotator());
		currentPath->InitPath(path);
	}


	//for (NodePosition pos : path)
	//{
	//	FVector center(pos.x, pos.y, 70);
	//	FVector extend(cellSize / 2.f);
	//	DrawDebugSolidBox(world, center, extend, FColor::Red, false, 100.f);
	//}
	//UE_LOG(LogTemp, Warning, TEXT("path length is %d"), (int)path.size());

	return currentPath;
}