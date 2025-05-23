// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soldier.generated.h"

UCLASS()
class BATTLEAI_API ASoldier : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void GoToFinalLocationAndOrientation(FVector commLocation, FRotator commOrientation);

	UPROPERTY(BlueprintReadOnly)
	FVector MyOffset;

	UPROPERTY(BlueprintReadOnly)
	class AFormationCommander* MyCommander;

	UPROPERTY(BlueprintReadOnly)
	class AFormation* MyFormation;
};
