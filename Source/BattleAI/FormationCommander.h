// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FormationCommander.generated.h"

UCLASS()
class BATTLEAI_API AFormationCommander : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFormationCommander();

	UPROPERTY(EditAnywhere)
	TArray<FVector> FormationPositions;

	UPROPERTY(BlueprintReadOnly)
	TArray<class ASoldier*> Soldiers;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitFormation();

	TSubclassOf<class ASoldier> SoldierRef;
	class AGlobalPath* GlobalPathRef;
	FVector FinalDestination;

	bool isMoving = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void SetupFinalDestination(FVector Location);

	UFUNCTION(BlueprintCallable)
	void MoveToLocation(FVector location);

	void SetGlobalPath(AGlobalPath* path) { GlobalPathRef = path; }
};