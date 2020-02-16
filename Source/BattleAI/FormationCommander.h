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

	TSubclassOf<class ASoldier> SoldierRef;

	UPROPERTY()
	class AGlobalPath* CurrentPath;

	TArray<FVector> FormationPositions; // describes the current relative offset for each soldier 
	int currentWidth, currentHeight;    // describes the current widht / height (num soldiers) of the formation

	float pathDelta = 0.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void AssignSoldierOffset(const AGlobalPath* path);

	UFUNCTION(BlueprintCallable)
	void MoveToLocation();

	UFUNCTION(BlueprintCallable)
	AGlobalPath* GetCurrentPath() const { return CurrentPath; }

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<class UFormationDescription> _formDescrRef; 
	
	void SetCurrentPath(AGlobalPath* path) { CurrentPath = path; }
	void GetFormationSize(float& width, float& height) const;
	void SetSelectionDisplay(bool selected);
};
