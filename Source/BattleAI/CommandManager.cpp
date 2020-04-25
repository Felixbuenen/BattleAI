// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandManager.h"
#include "FormationFrame.h"

#include "FormationCommander.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
ACommandManager::ACommandManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACommandManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommandManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	float xPos, yPos;
	PlayerController->GetMousePosition(xPos, yPos);

}

void ACommandManager::InitTargetLocationDisplay()
{
	const TArray<TArray<FVector>>& targetSoldierLocations = frame->GetTargetSoldierLocations();
	for (const auto& comm : targetSoldierLocations)
	{
		for (const auto& sold : comm)
		{
			//UDecalComponent* decal = NewObject<UDecalComponent>(this, FName("decal"));
			UDecalComponent* decal = UGameplayStatics::SpawnDecalAttached(targetLocDecalRef, FVector(50.f, 50.f, 50.f), RootComponent);
			decal->DecalSize = FVector(50.f, 50.f, 200.f);
			decal->SetVisibility(true);

			targetLocDecals.Emplace(decal);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Num decals: %d"), targetLocDecals.Num());
}

void ACommandManager::StopTargetLocationDisplay()
{
	for (auto decal : targetLocDecals)
	{
		decal->DestroyComponent();
	}
	targetLocDecals.Empty();
}

void ACommandManager::UpdateTargetLocationDisplay()
{
	if (targetLocDecals.Num() == 0) return;

	const TArray<TArray<FVector>>& targetSoldierLocations = frame->GetTargetSoldierLocations();
	const TArray<FVector>& commLocation = frame->GetCommanderTargetLocations();
	const FRotator& targetRot = frame->GetTargetRotation();
	
	int counter = 0;
	int numCommanders = targetSoldierLocations.Num();
	for (int i = 0; i < numCommanders; i++)
	{
		int numSoldiers = targetSoldierLocations[i].Num();
		for(int s = 0; s < numSoldiers; s++)
		{
			int offset = numSoldiers * i;
			FVector targetLoc = targetRot.RotateVector(targetSoldierLocations[i][s]);
			targetLoc += commLocation[i];
			targetLoc.Z = 0.f;
			targetLocDecals[offset + s]->SetWorldRotation(targetRot);
			targetLocDecals[offset + s]->SetWorldLocation(targetLoc);

		}
	}
}

void ACommandManager::HandleLeftMouseDown(AFormationCommander* formation)
{
	// valid formation selected
	if (formation)
	{
		int formationIndex = activeFormations.Find(formation);
		bool alreadyActive = formationIndex != INDEX_NONE;

		if (alreadyActive)
		{
			if (multiSelect)
			{
				ToggleSelectFormation(false, formation, formationIndex);
			}
			else
			{
				DeselectAllFormations();
				ToggleSelectFormation(true, formation);
			}
		}
		else
		{
			if (!multiSelect)
			{
				DeselectAllFormations();
			}

			ToggleSelectFormation(true, formation);
		}

		
	}
	// nothing selected
	else
	{
		if (!multiSelect)
		{
			DeselectAllFormations();
		}
	}
}

bool ACommandManager::ValidFormationPosition() const
{
	return frame->GetPositionIsValid();
}

void ACommandManager::ToggleSelectFormation(bool selected, AFormationCommander* formation, int index)
{
	formation->SetSelectionDisplay(selected);
	if (selected)
	{
		activeFormations.Emplace(formation);
		//frame->UpdateActiveFormations(activeFormations);
	}
	else
	{
		activeFormations.Remove(formation);
		//frame->UpdateActiveFormations(activeFormations);
	}
}

void ACommandManager::DeselectAllFormations()
{
	for (auto f : activeFormations)
	{
		f->SetSelectionDisplay(false);
	}

	activeFormations.Empty();
	//frame->UpdateActiveFormations(activeFormations);
}
