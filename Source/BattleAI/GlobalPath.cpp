// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalPath.h"
#include "AStarSolver.h"
#include "DrawDebugHelpers.h"
#include "BattleAIGameState.h"

#include "Components/SplineComponent.h"

// Sets default values
AGlobalPath::AGlobalPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	splinePath = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Path"));
	SetRootComponent(splinePath);
}

// Called when the game starts or when spawned
void AGlobalPath::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGlobalPath::InitPath(std::vector<NodePosition>& pathPoints)
{
	pathPositions = pathPoints;
	
	// ensure path is empty at this point
	splinePath->ClearSplinePoints(true);
	
	int pathLength = (int)pathPositions.size();
	
	for (int i = pathLength - 1; i >= 0; i--)
	{
		FVector position;
		position.X = pathPositions[i].x;
		position.Y = pathPositions[i].y;
	
		splinePath->AddSplinePoint(position, ESplineCoordinateSpace::World ,false);
		//splinePath->SetTangentAtSplinePoint(i, FVector{150.f, 0.f, 0.f}, ESplineCoordinateSpace::Local, true);
	}
	
	splinePath->UpdateSpline();
	splinePath->Duration = 1.f;
}

FRotator AGlobalPath::GetDirectionAtPercentile(float percentile) const
{
	return splinePath->GetRotationAtTime(percentile, ESplineCoordinateSpace::World);
}
FVector AGlobalPath::GetLocationAtPercentile(float percentile) const
{
	return splinePath->GetLocationAtTime(percentile, ESplineCoordinateSpace::World);
}

float AGlobalPath::GetPathLength() const
{
	return splinePath->GetSplineLength();
}

// Called every frame
void AGlobalPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//-------------- TODO: fix dit --------------

	//bool showPath = Cast<ABattleAIGameState>(GetWorld()->GetGameState())->ShowGlobalPaths;
	//if (showPath)
	//{
	//	for (const NodePosition& pos : pathPositions)
	//	{
	//		FVector center(pos.x, pos.y, 70);
	//		FVector extend(CellSize * 0.5f);
	//		DrawDebugSolidBox(GetWorld(), center, extend, FColor::Red, false);
	//	}
	//}


}

