// Fill out your copyright notice in the Description page of Project Settings.


#include "Formation.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Engine/World.h"
#include "UObject/UObjectGlobals.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "FormationCommander.h"
#include "FormationDescription.h"
#include "Soldier.h"
#include "GlobalPath.h"
#include "HungarianAlgorithm.h"

// Sets default values
AFormation::AFormation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UClass> soldierReference(TEXT("Blueprint'/Game/Blueprints/Soldier_BP.Soldier_BP_C'"));
	SoldierRef = soldierReference.Object;

	static ConstructorHelpers::FObjectFinder<UClass> commanderReference(TEXT("Blueprint'/Game/AI/FormationCommander_BP.FormationCommander_BP_C'"));
	CommanderRef = commanderReference.Object;
}

// Called when the game starts or when spawned
void AFormation::BeginPlay()
{
	Super::BeginPlay();

	// spawn the formation commander
	commander = GetWorld()->SpawnActor<AFormationCommander>(CommanderRef, GetActorLocation(), GetActorRotation());
	commander->myFormation = this;

	InitFormation();
}

// Called every frame
void AFormation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFormation::InitFormation()
{
	// get formation details from formation description
	int width = _formDescrRef.GetDefaultObject()->defaultWidth;
	int height = _formDescrRef.GetDefaultObject()->defaultHeight;
	float clearance = _formDescrRef.GetDefaultObject()->agentClearance;

	float halfWidth = (float)(width - 1) * 0.5f;
	float halfHeight = (float)(height - 1) * 0.5f;

	// loop through all required soldiers and spawn / store them
	for (int y = 0; y < width; y++)
	{
		for (int x = 0; x < height; x++)
		{
			FRotator rotation = GetActorRotation();

			FVector relOffset = FVector(((float)x - halfHeight) * clearance, ((float)y - halfWidth) * clearance, 0.f);
			FVector location = GetActorLocation() + rotation.RotateVector(relOffset);

			auto soldier = GetWorld()->SpawnActor<ASoldier>(SoldierRef, location, rotation);
			if (!soldier)
			{
				UE_LOG(LogTemp, Error, TEXT("Couldn't create soldier"));
				return;
			}
			soldier->MyOffset = relOffset;
			soldier->MyCommander = commander;
			soldier->MyFormation = this;

			// store data
			Soldiers.Emplace(soldier);
			FormationPositions.Emplace(relOffset);
			currentWidth = width;
			currentHeight = height;
		}
	}
}

void AFormation::AssignSoldierOffset(const AGlobalPath* path)
{
	if (!IsValid(path))
	{
		UE_LOG(LogTemp, Warning, TEXT("NO PATH!"));
		return;
	}
	FRotator dirRotation = path->GetDirectionAtVertex(0); // get starting direction
	UE_LOG(LogTemp, Warning, TEXT("DIR: %s"), *dirRotation.Vector().ToString());

	AssignSoldierOffset(dirRotation);
}

void AFormation::AssignSoldierOffset(FRotator orientation) const
{
	// TODO: optimize!
	HungarianAlgorithm ha;
	std::vector<std::vector<double>> haMatrix;
	std::vector<int> assignmentOutput;
	std::vector<FVector> targetPositions;

	// fill (rotated) target positions
	int numSoldiers = FormationPositions.Num();
	for (int i = 0; i < numSoldiers; i++)
	{
		FVector relLocation = orientation.RotateVector(FormationPositions[i]);
		//FVector relLocation = FormationPositions[i];
		FVector targetPos = relLocation + commander->GetActorLocation();
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

	UE_LOG(LogTemp, Warning, TEXT("Hungarian algorithm solved"));
}



void AFormation::GetFormationSize(float& width, float& height) const
{
	width = FormBbox.Y;
	height = FormBbox.X;
}

void AFormation::SetSelectionDisplay(bool selected)
{
	for (const ASoldier* s : Soldiers)
	{
		UDecalComponent* decal = (UDecalComponent*)s->FindComponentByClass<UDecalComponent>();
		decal->SetVisibility(selected);
	}
}

void AFormation::SetSoldierMovementSpeed(float speed)
{
	int numSoldiers = GetNumSoldiers();
	for (int i = 0; i < numSoldiers; i++)
	{
		Soldiers[i]->GetCharacterMovement()->MaxWalkSpeed = 200.f; // TODO: hard coded for now, make smarter
	}
}

void AFormation::SetSoldierTargetLocationAndOrientation(const FVector& location, const FRotator& rotation)
{
	int numSoldiers = GetNumSoldiers();
	for (int i = 0; i < numSoldiers; i++)
	{
		Soldiers[i]->GoToFinalLocationAndOrientation(location, rotation);
	}
}

UFormationDescription* AFormation::GetFormationDescription() const
{
	return _formDescrRef.GetDefaultObject();
}