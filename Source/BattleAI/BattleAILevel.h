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

	class UAStarSolver* pathfinder;
	class AGlobalPath* currentPath;

	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	void InitPathfindingInfo();

	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	class AGlobalPath* FindGlobalPath(FVector commanderPosition, FVector destination);

	UFUNCTION(BlueprintImplementableEvent, Category = LevelBlueprint)
	int GetClearance(FVector position, FVector extent);

};