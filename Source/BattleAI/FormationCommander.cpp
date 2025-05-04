// Fill out your copyright notice in the Description page of Project Settings.

#include "FormationCommander.h"

#include "Soldier.h"
#include "HungarianAlgorithm.h"
#include "GlobalPath.h"
#include "FormationDescription.h"
#include "Formation.h"

#include "Engine/World.h"
#include "Engine.h"
#include "Kismet/KismetSystemLibrary.h" 

AFormationCommander::AFormationCommander()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Sets default values
AFormationCommander::AFormationCommander(AFormation* formation)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	myFormation = formation;
}

// Called when the game starts or when spawned
void AFormationCommander::BeginPlay()
{
	Super::BeginPlay();

	CurrentPath = GetWorld()->SpawnActor<AGlobalPath>(AGlobalPath::StaticClass(), FVector(0.f), FRotator());
}

void AFormationCommander::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Warning, TEXT("Actor %s destroyed. Reason: %d"), *GetName(), (int32)EndPlayReason);
}


// Called every frame
void AFormationCommander::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move(DeltaTime);
}

// Called to bind functionality to input
void AFormationCommander::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFormationCommander::TriggerMoveToLocation()
{
	isMoving = true;
	pathDelta = 0.f; // ugly... should probably be included in the GlobalPath object
}

void AFormationCommander::GoToLocation(const FVector& location, class AGlobalPath* path)
{
	if (!IsValid(path))
	{
		UE_LOG(LogTemp, Warning, TEXT("path not valid"));
	}
	//CurrentPath = path;
	myFormation->AssignSoldierOffset(path);
	targetLocation = location;

	TriggerMoveToLocation();
}

void AFormationCommander::Move(float DeltaTime)
{
	if (isMoving)
	{
		if (pathDelta > 0.995f) // ugly... should probably be included in the GlobalPath object
		{
			isMoving = false;
			MoveToOrientation();

			return;
		}
		FRotator dir = CurrentPath->GetDirectionAtPercentile(pathDelta);
		FVector pos = CurrentPath->GetLocationAtPercentile(pathDelta);
		pos.Z = GetActorLocation().Z;
		dir.Roll = 0.f;
		dir.Pitch = 0.f;

		SetActorLocationAndRotation(pos, dir);

		// -----------------TODO: make speed variable, depending on max soldier distance
		pathDelta += ((150.f * DeltaTime) / CurrentPath->GetPathLength());
	}
}

void AFormationCommander::MoveToOrientation()
{
	// set final position / orientation of commander
	TargetRotation.Roll = 0.f;
	TargetRotation.Pitch = 0.f;
	TargetLocation.Z = GetActorLocation().Z;
	SetActorLocationAndRotation(TargetLocation, TargetRotation);

	// update soldier relative offset
	myFormation->AssignSoldierOffset(TargetRotation);

	// order soldiers to go to their final location and go into orientation
	myFormation->SetSoldierMovementSpeed(200.0f);
	myFormation->SetSoldierTargetLocationAndOrientation(TargetLocation, TargetRotation);
}