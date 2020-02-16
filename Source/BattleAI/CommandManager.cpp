// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandManager.h"
#include "FormationFrame.h"

#include "FormationCommander.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


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
