// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandManager.h"
#include "FormationFrame.h"
#include "FormationFrameDrawer.h"
#include "FormationCommander.h"
#include "Formation.h"

#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"

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

	if (!activeFormationFrame) return;
	if (formationFrameDragging)
	{
		frame->Update(GetWorldCursorLocation());
		return;
	}

	// get mouse screen position
	APlayerController* plController = GetWorld()->GetFirstPlayerController();
	FVector2D currentScreenPos;
	plController->GetMousePosition(currentScreenPos.X, currentScreenPos.Y);

	// check if mouse was click-dragged far enough (if so, trigger the formation frame)
	float screenDist = (frameStartDragScreenPosition - currentScreenPos).Size();
	if (screenDist >= screenDragFrameTriggerTreshold)
	{
		formationFrameDragging = true;
		frame->Update(GetWorldCursorLocation());
	}
}

void ACommandManager::ToggleSelectFormation(AFormation* formation)
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

void ACommandManager::ToggleSelectFormation(bool selected, AFormation* formation, int index)
{
	formation->SetSelectionDisplay(selected);
	if (selected)
	{
		activeFormations.Emplace(formation);
	}
	else
	{
		activeFormations.Remove(formation);
	}
}

void ACommandManager::StartDragFormationGoal()
{
	activeFormationFrame = true;

	// get mouse screen position
	APlayerController* plController = GetWorld()->GetFirstPlayerController();
	plController->GetMousePosition(frameStartDragScreenPosition.X, frameStartDragScreenPosition.Y);

	// get formation click location
	FVector hitWorldLocation = GetWorldCursorLocation();

	frame->Init(hitWorldLocation, hitWorldLocation, activeFormations);
}

void ACommandManager::StopDragFormationGoal()
{
	activeFormationFrame = false;
	formationFrameDragging = false;
	frame->Stop(false);
}

void ACommandManager::DeselectAllFormations()
{
	for (auto f : activeFormations)
	{
		f->SetSelectionDisplay(false);
	}

	activeFormations.Empty();
}

const FVector ACommandManager::GetWorldCursorLocation() const
{
	APlayerController* plController = GetWorld()->GetFirstPlayerController();

	FHitResult outHit;
	plController->GetHitResultUnderCursorByChannel(ETraceTypeQuery(ECollisionChannel::ECC_EngineTraceChannel3), true, outHit);
	return outHit.Location;
}