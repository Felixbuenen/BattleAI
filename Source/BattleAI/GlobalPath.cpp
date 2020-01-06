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
	if (splinePath)
	{
		splinePath->ClearSplinePoints(true);
		
		int pathLength = (int)pathPoints.size();

		for (int i = pathLength - 1; i >= 0; i--)
		{
			FVector position;
			position.X = pathPoints[i].x;
			position.Y = pathPoints[i].y;

			splinePath->AddSplinePoint(position, ESplineCoordinateSpace::World ,true);
			//splinePath->SetTangentAtSplinePoint(i, FVector{150.f, 0.f, 0.f}, ESplineCoordinateSpace::Local, true);
		}

		splinePath->UpdateSpline();
		splinePath->Duration = 1.f;
	}
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

}

