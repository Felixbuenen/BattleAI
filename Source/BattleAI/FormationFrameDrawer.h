// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "FormationFrameDrawer.generated.h"

#define RED_HUE 0.33333

/**
 * Responsible for drawing the target locations of a soldier formation when click-dragging a formation in position.
 */
UCLASS()
class BATTLEAI_API UFormationFrameDrawer : public UObject
{
	GENERATED_BODY()

public:
	UFormationFrameDrawer();

	void InitDrawer(class USceneComponent* drawRoot, class AFormationFrame* frame);

	void InitTargetLocationDisplay();
	void StopTargetLocationDisplay();
	void UpdateTargetLocationDisplay();

	class UMaterial* targetLocDecalRef;

private:
	
	UPROPERTY()
	TArray<class UDecalComponent*> targetLocDecals;

	class AFormationFrame* _frame;
	class USceneComponent* _drawRoot;
};
