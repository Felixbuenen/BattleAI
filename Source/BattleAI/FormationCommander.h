// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FormationCommander.generated.h"

UCLASS()
class BATTLEAI_API AFormationCommander : public ACharacter
{
	GENERATED_BODY()

	friend class AFormation;

public:
	// Sets default values for this character's properties
	AFormationCommander();
	AFormationCommander(class AFormation* formation);

	UPROPERTY(BlueprintReadOnly)
	bool isMoving = false;

	// TODO: this should be readable from current path
	UPROPERTY(BlueprintReadOnly)
	FVector targetLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(float DeltaTime);

	UPROPERTY()
	class AGlobalPath* CurrentPath;

	float pathDelta = 0.f;

	FVector TargetLocation;
	FRotator TargetRotation;

	class AFormation* myFormation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// --- MOVEMENT ---
	UFUNCTION(BlueprintCallable)
	void TriggerMoveToLocation();
	void MoveToOrientation();

	UFUNCTION(BlueprintCallable)
		void GoToLocation(const FVector& location, class AGlobalPath* path);

	UFUNCTION(BlueprintCallable)
	AGlobalPath* GetCurrentPath() const { return CurrentPath; }
	void SetCurrentPath(AGlobalPath* path) { CurrentPath = path; }

	UFUNCTION(BlueprintCallable)
	FVector GetTargetLocation() const { return TargetLocation; }
	void SetTargetLocation(FVector targetLocation) { TargetLocation = targetLocation; }

	UFUNCTION(BlueprintCallable)
	const FRotator& GetTargetRotation() const { return TargetRotation; }
	void SetTargetRotation(FRotator targetRotation) { TargetRotation = targetRotation; }

	UFUNCTION(BlueprintCallable)
		class AFormation* GetFormation() const { return myFormation; }
};
