// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BattleAIGameState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAI_API ABattleAIGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debugging)
	//bool ShowSDF = false;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debugging)
	//bool ShowGlobalPaths = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debugging)
	bool ShowSoldierDestinations = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Debugging)
	bool ShowSoldierInterDestinations = false;
};
