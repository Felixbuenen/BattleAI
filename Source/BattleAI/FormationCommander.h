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

	UPROPERTY(BlueprintReadOnly)
	TArray<class ASoldier*> Soldiers;

	UPROPERTY(BlueprintReadOnly)
	bool isMoving = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitFormation();
	void Move(float DeltaTime);
	void AssignSoldierOffsetInternal(FRotator orientation) const;

	TSubclassOf<class ASoldier> SoldierRef;

	UPROPERTY()
	class AGlobalPath* CurrentPath;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<class UFormationDescription> _formDescrRef;

	TArray<FVector> FormationPositions; // describes the current relative offset for each soldier 
	int currentWidth, currentHeight;    // describes the current widht / height (num soldiers) of the formation

	float pathDelta = 0.f;

	FVector TargetLocation;
	FRotator TargetRotation;
	FVector2D FormBbox;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void AssignSoldierOffset(const AGlobalPath* path);

	UFUNCTION(BlueprintCallable)
	void MoveToLocation();

	void MoveToOrientation();

	UFUNCTION(BlueprintCallable)
	AGlobalPath* GetCurrentPath() const { return CurrentPath; }

	UFormationDescription* GetFormationDescription() const;
	int GetNumSoldiers() const { return (int)Soldiers.Num(); }
	
	void SetCurrentPath(AGlobalPath* path) { CurrentPath = path; }
	void GetFormationSize(float& width, float& height) const;
	void SetSelectionDisplay(bool selected);

	UFUNCTION(BlueprintCallable)
	FVector GetTargetLocation() const { return TargetLocation; }
	void SetTargetLocation(FVector targetLocation) { TargetLocation = targetLocation; }

	UFUNCTION(BlueprintCallable)
	const TArray<FVector>& GetFormaitonPositions() const { return FormationPositions; }
	void SetFormationPositions(const TArray<FVector>& formPos) { FormationPositions = formPos; }

	UFUNCTION(BlueprintCallable)
	const FRotator& GetTargetRotation() const { return TargetRotation; }
	void SetTargetRotation(FRotator targetRotation) { TargetRotation = targetRotation; }

	const FVector2D& GetFormationBoundingBox() const { return FormBbox; }
	void SetFormationBoundingBox(FVector2D formBbox) { FormBbox = formBbox; }
};
