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
	
public:
	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	class UPathPlanner* GetPathPlanner() const;

protected:

	UPROPERTY(EditAnywhere)
	class AStaticMeshActor* floor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> obstacle;

	UPROPERTY(EditAnywhere)
	class UPathPlanner* pathfinder;

	UPROPERTY(EditAnywhere)
	bool DivideAndConquerPathfinding;

	UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
	void InitPathfindingInfo();

	//UFUNCTION(BlueprintCallable, Category = DebugDrawing)
	void DrawPlannerInfo(int infoKey) const;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = DebugDrawing)
	void ToggleRenderDebugItem(int item);

private:
	TArray<int> drawDebugKeys; // TODO: make enmum
};
