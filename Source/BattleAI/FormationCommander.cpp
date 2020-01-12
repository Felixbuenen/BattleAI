// Fill out your copyright notice in the Description page of Project Settings.

#include "FormationCommander.h"
#include "Soldier.h"
#include "HungarianAlgorithm.h"
#include "GlobalPath.h"

#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"
#include "Kismet/KismetSystemLibrary.h" 

// Sets default values
AFormationCommander::AFormationCommander()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> FloorCellClassFinder(TEXT("Blueprint'/Game/Blueprints/Soldier_BP.Soldier_BP_C'"));
	SoldierRef = FloorCellClassFinder.Object;
}



void AFormationCommander::InitFormation()
{
	// spawn soldiers and set correct orientation

	int numSoldiers = FormationPositions.Num();
	for (int i = 0; i < numSoldiers; i++)
	{
		FVector location = GetActorLocation() + FormationPositions[i];
		FRotator rotation = GetActorRotation();

		auto soldier = GetWorld()->SpawnActor<ASoldier>(SoldierRef, location, rotation);
		soldier->MyOffset = FormationPositions[i];
		Soldiers.Add( soldier);
	}

	//FinalDestination = GetActorLocation();
}

// Called when the game starts or when spawned
void AFormationCommander::BeginPlay()
{
	Super::BeginPlay();

	CurrentPath = GetWorld()->SpawnActor<AGlobalPath>(AGlobalPath::StaticClass(), FVector(0.f), FRotator());

	InitFormation();
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

void AFormationCommander::MoveToLocation()
{
	isMoving = true;
	pathDelta = 0.f; // ugly...
}

void AFormationCommander::Move(float DeltaTime)
{
	if (isMoving)
	{
		if (pathDelta > 0.995f) // ugly...
		{
			isMoving = false;
			return;
		}
		FRotator dir = CurrentPath->GetDirectionAtPercentile(pathDelta);
		FVector pos = CurrentPath->GetLocationAtPercentile(pathDelta);
		pos.Z = GetActorLocation().Z;
		dir.Roll = 0.f;
		dir.Pitch = 0.f;

		SetActorLocationAndRotation(pos, dir);

		pathDelta += ((150.f * DeltaTime) / CurrentPath->GetPathLength());
	}
}

void AFormationCommander::AssignSoldierOffset(const AGlobalPath* path)
{
	// indicates where the formation is facing at final location
	//FVector2D dir = ((FVector2D)Location - (FVector2D)GetActorLocation());
	//FinalDestination = Location;

	FRotator dirRotation = path->GetDirectionAtPercentile(0.f); // get starting direction
	FVector2D dir = (FVector2D)dirRotation.Vector();

	UE_LOG(LogTemp, Error, TEXT("Direction: %s"), *dir.ToString());

	// TODO: optimize!
	HungarianAlgorithm ha;
	std::vector<std::vector<double>> haMatrix;
	std::vector<int> assignmentOutput;
	std::vector<FVector> targetPositions;
	FVector2D rightOrientation;
	rightOrientation.X = -dir.Y;
	rightOrientation.Y = dir.X;
	
	// fill (rotated) target positions
	int numSoldiers = FormationPositions.Num();
	for (int i = 0; i < numSoldiers; i++)
	{
		FVector targetPos = FVector((dir * FormationPositions[i].X + rightOrientation * FormationPositions[i].Y), 0) + GetActorLocation();
		targetPositions.push_back(targetPos);
	}
	
	// fill the assignment matrix (hungarian algorithm)
	for (int row = 0; row < numSoldiers; row++)
	{
		haMatrix.push_back(std::vector<double>());
		FVector currentPos = Soldiers[row]->GetActorLocation();

		for (int col = 0; col < numSoldiers; col++)
		{
			double distance = (currentPos - targetPositions[col]).Size(); // maybe size sqrd for optimization?
			haMatrix[row].push_back(distance);
		}
	}

	// calculate soldier position assignment
	ha.Solve(haMatrix, assignmentOutput);

	// assign new offsets to soldiers
	for (int i = 0; i < numSoldiers; i++)
	{
		Soldiers[i]->MyOffset = FormationPositions[assignmentOutput[i]];
	}

	//const UWorld* world = GetWorld();
	//for (int i = 0; i < numSoldiers; i++)
	//{
	//	FVector center = FormationPositions[assignmentOutput[i]] + GetActorLocation();
	//	center.Z = 60.f;
	//	DrawDebugBox(world, center, FVector{100.f}, FColor::Magenta, false, 100.f);
	//}
}
