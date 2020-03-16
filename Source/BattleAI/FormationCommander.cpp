// Fill out your copyright notice in the Description page of Project Settings.

#include "FormationCommander.h"
#include "Soldier.h"
#include "HungarianAlgorithm.h"
#include "GlobalPath.h"
#include "FormationDescription.h"

#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Components/DecalComponent.h"

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
	// get formation details from formation description
	int width = _formDescrRef.GetDefaultObject()->defaultWidth;
	int height = _formDescrRef.GetDefaultObject()->defaultHeight;
	float clearance = _formDescrRef.GetDefaultObject()->agentClearance;

	float halfWidth = (float)(width-1) * 0.5f;
	float halfHeight = (float)(height-1) * 0.5f;

	// loop through all required soldiers and spawn / store them
	for (int y = 0; y < width; y++)
	{
		for (int x = 0; x < height; x++)
		{
			FRotator rotation = GetActorRotation();

			FVector relOffset = FVector(((float)x - halfHeight) * clearance, ((float)y - halfWidth) * clearance, 0.f);
			FVector location = GetActorLocation() + rotation.RotateVector(relOffset);

			auto soldier = GetWorld()->SpawnActor<ASoldier>(SoldierRef, location, rotation);
			soldier->MyOffset = relOffset;
			soldier->MyCommander = this;
			
			// store data
			Soldiers.Emplace(soldier);
			FormationPositions.Emplace(relOffset);
			currentWidth = width;
			currentHeight = height;
		}
	}
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
	pathDelta = 0.f; // ugly... should probably be included in the GlobalPath object
}

void AFormationCommander::Move(float DeltaTime)
{
	if (isMoving)
	{
		if (pathDelta > 0.995f) // ugly... should probably be included in the GlobalPath object
		{
			isMoving = false;

			// for now: directly set rotation
			//SetActorRotation(CurrentPath->GetDirectionAtPercentile(1.f));
			MoveToOrientation();

			return;
		}
		FRotator dir = CurrentPath->GetDirectionAtPercentile(pathDelta);
		FVector pos = CurrentPath->GetLocationAtPercentile(pathDelta);
		pos.Z = GetActorLocation().Z;
		dir.Roll = 0.f;
		dir.Pitch = 0.f;

		SetActorLocationAndRotation(pos, dir);

		//UE_LOG(LogTemp, Warning, TEXT("first pos: %s"), *CurrentPath->GetLocationAtPercentile(0.5f).ToString());

		// -----------------TODO: make speed variable, depending on max soldier distance
		pathDelta += ((150.f * DeltaTime) / CurrentPath->GetPathLength());
	}
}

void AFormationCommander::MoveToOrientation()
{
	TargetRotation.Roll = 0.f;
	TargetRotation.Pitch = 0.f;
	TargetLocation.Z = GetActorLocation().Z;
	SetActorLocationAndRotation(TargetLocation, TargetRotation);

	// order soldiers to go to their final location and go into orientation
	int numSoldiers = Soldiers.Num();
	for (int i = 0; i < numSoldiers; i++)
	{
		Soldiers[i]->GoToFinalLocationAndOrientation(TargetLocation, TargetRotation);
	}
}


void AFormationCommander::AssignSoldierOffset(const AGlobalPath* path)
{
	FRotator dirRotation = path->GetDirectionAtPercentile(0.f); // get starting direction

	// TODO: optimize!
	HungarianAlgorithm ha;
	std::vector<std::vector<double>> haMatrix;
	std::vector<int> assignmentOutput;
	std::vector<FVector> targetPositions;
	
	// fill (rotated) target positions
	int numSoldiers = FormationPositions.Num();
	for (int i = 0; i < numSoldiers; i++)
	{
		FVector relLocation = dirRotation.RotateVector(FormationPositions[i]);
		FVector targetPos = relLocation + GetActorLocation();
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
}

void AFormationCommander::GetFormationSize(float& width, float& height) const
{
	float clearance = _formDescrRef.GetDefaultObject()->agentClearance;
	
	width = currentWidth * clearance;
	height = currentHeight * clearance;
}

void AFormationCommander::SetSelectionDisplay(bool selected)
{
	for (ASoldier* s : Soldiers)
	{
		UDecalComponent* decal = (UDecalComponent*)s->FindComponentByClass<UDecalComponent>();
		decal->SetVisibility(selected);
	}
}

UFormationDescription* AFormationCommander::GetFormationDescription() const
{
	return _formDescrRef.GetDefaultObject();
}