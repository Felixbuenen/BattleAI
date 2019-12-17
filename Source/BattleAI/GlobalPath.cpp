// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalPath.h"
#include "AStarSolver.h"

#include "Components/SplineComponent.h"

// Sets default values
AGlobalPath::AGlobalPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	splinePath = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Path"));
}

// Called when the game starts or when spawned
void AGlobalPath::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGlobalPath::InitPath(const std::vector<NodePosition>& pathPoints)
{
	// ensure path is empty at this point
	splinePath->ClearSplinePoints(false);

	// set spline path points
	for (const NodePosition& pos : pathPoints)
	{
		FVector position;
		position.X = pos.x;
		position.Y = pos.y;
		splinePath->AddSplinePoint(position, ESplineCoordinateSpace::World, false);
	}

	splinePath->UpdateSpline();
	splinePath->Duration = 1.f;
}

FVector AGlobalPath::GetDirectionAtPercentile(float percentile) const
{
	return splinePath->GetDirectionAtTime(percentile, ESplineCoordinateSpace::World);
}
FVector AGlobalPath::GetLocationAtPercentile(float percentile) const
{
	return splinePath->GetLocationAtTime(percentile, ESplineCoordinateSpace::World);
}

// Called every frame
void AGlobalPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

