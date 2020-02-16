// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FormationDescription.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class BATTLEAI_API UFormationDescription : public UObject
{
	GENERATED_BODY()
public:
	UFormationDescription() {}

	// simple description of a square formation
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int defaultWidth;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int defaultHeight;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int minWidth;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int minHeight;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float agentClearance; // ignores agent radius, aka a value of 0 means soldiers overlap!
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int maxSoldiers;

	// returns, given a certain width, the appropriate formation
	TArray<FVector2D> GetFormationFromWidth(float width, int numSoldiers, FVector2D& outBBox);
};
