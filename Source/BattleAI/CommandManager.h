// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommandManager.generated.h"

UCLASS()
class BATTLEAI_API ACommandManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACommandManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HandleLeftMouseDown(class AFormationCommander* formation);

	UFUNCTION(BlueprintCallable)
	void HandleRightMouseDown();

	UFUNCTION(BlueprintCallable)
	void HandleRightMouseUp();

	UFUNCTION(BlueprintCallable)
	void HandleMouseMoved(FVector2D screenPos);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};