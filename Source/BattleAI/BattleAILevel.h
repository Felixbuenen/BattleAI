// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "BattleAILevel.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAI_API ABattleAILevel : public ALevelScriptActor
{
	GENERATED_BODY()

	ABattleAILevel();
	
protected:

	UPROPERTY(EditAnywhere)
	class AStaticMeshActor* floor;

	UPROPERTY()
	class UAStarSolver* pathfinder;

	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	void InitPathfindingInfo();

	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	class AGlobalPath* FindGlobalPath(class AFormationCommander* commander, FVector destination);

	UFUNCTION(BlueprintImplementableEvent, Category = LevelBlueprint)
	int CalculateClearance(FVector position, FVector extent);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int gridDimension = 110;
	
	float CellExtent;
	float levelBoundX;
	float levelBoundY;
};
