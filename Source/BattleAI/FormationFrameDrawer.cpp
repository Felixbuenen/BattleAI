// Fill out your copyright notice in the Description page of Project Settings.

#include "FormationFrameDrawer.h"

#include "Materials/MaterialInterface.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Components/SceneComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "FormationFrame.h"

UFormationFrameDrawer::UFormationFrameDrawer()
{
	ConstructorHelpers::FObjectFinder<UMaterial> selectDecal(TEXT("/Game/Materials/soldier_selected_decal"));
	targetLocDecalRef = selectDecal.Object;
}

void UFormationFrameDrawer::InitDrawer(USceneComponent* drawRoot, AFormationFrame* frame)
{
	_frame = frame;
	_drawRoot = drawRoot;
}

void UFormationFrameDrawer::InitTargetLocationDisplay()
{
	const TArray<TArray<FVector>>& targetSoldierLocations = _frame->GetTargetSoldierLocations();
	for (const auto& comm : targetSoldierLocations)
	{
		for (const auto& sold : comm)
		{
			//UDecalComponent* decal = NewObject<UDecalComponent>(this, FName("decal"));
			UDecalComponent* decal = UGameplayStatics::SpawnDecalAttached(targetLocDecalRef, FVector(50.f, 50.f, 50.f), _drawRoot);
			decal->DecalSize = FVector(50.f, 50.f, 200.f);
			decal->SetVisibility(true);
			decal->CreateDynamicMaterialInstance();

			targetLocDecals.Emplace(decal);
		}
	}
}

void UFormationFrameDrawer::StopTargetLocationDisplay()
{
	for (auto decal : targetLocDecals)
	{
		decal->DestroyComponent();
	}
	targetLocDecals.Empty();
}

// draws the soldier location decals
void UFormationFrameDrawer::UpdateTargetLocationDisplay()
{
	if (targetLocDecals.Num() == 0) return;

	const TArray<TArray<FVector>>& targetSoldierLocations = _frame->GetTargetSoldierLocations();
	const TArray<FVector>& commLocation = _frame->GetCommanderTargetLocations();
	const FRotator& targetRot = _frame->GetTargetRotation();
	const bool validPosition = _frame->GetValidPosition();

	float hue = validPosition ? 0.f : RED_HUE;

	int counter = 0;
	int numCommanders = targetSoldierLocations.Num();
	for (int i = 0; i < numCommanders; i++)
	{
		int numSoldiers = targetSoldierLocations[i].Num();
		for (int s = 0; s < numSoldiers; s++)
		{
			int offset = numSoldiers * i;
			FVector targetLoc = targetRot.RotateVector(targetSoldierLocations[i][s]);
			targetLoc += commLocation[i];
			targetLoc.Z = 0.f;
			targetLocDecals[offset + s]->SetWorldRotation(targetRot);
			targetLocDecals[offset + s]->SetWorldLocation(targetLoc);

			UMaterialInstanceDynamic* matRef = (UMaterialInstanceDynamic*)targetLocDecals[offset + s]->GetMaterial(0);
			if (matRef != nullptr)
			{
				matRef->SetScalarParameterValue("Hue", hue);
			}
		}
	}
}