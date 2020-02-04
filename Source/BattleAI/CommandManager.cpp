// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandManager.h"

#include "FormationCommander.h"

// Sets default values
ACommandManager::ACommandManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

void ACommandManager::HandleRightMouseDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Down"));
}

void ACommandManager::HandleRightMouseUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Up"));
}

void ACommandManager::HandleMouseMoved(FVector2D screenPos)
{

}

void ACommandManager::ToggleSelectFormation(bool selected, AFormationCommander* formation, int index)
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

void ACommandManager::DeselectAllFormations()
{
	for (auto f : activeFormations)
	{
		f->SetSelectionDisplay(false);
	}

	activeFormations.Empty();
}
