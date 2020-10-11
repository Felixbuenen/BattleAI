// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Formation.generated.h"

UCLASS()
class BATTLEAI_API AFormation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFormation();

	void InitFormation();

	UFUNCTION(BlueprintCallable)
	void AssignSoldierOffset(const class AGlobalPath* path);

	UPROPERTY(BlueprintReadOnly)
	TArray<class ASoldier*> Soldiers;

	class UFormationDescription* GetFormationDescription() const;
	
	FORCEINLINE int GetNumSoldiers() const { return (int)Soldiers.Num(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE class AFormationCommander* GetCommander() const { return commander; };

	void GetFormationSize(float& width, float& height) const;
	void SetSelectionDisplay(bool selected);

	UFUNCTION(BlueprintCallable)
	const TArray<FVector>& GetFormationPositions() const { return FormationPositions; }
	void SetFormationPositions(const TArray<FVector>& formPos) { FormationPositions = formPos; }

	const FVector2D& GetFormationBoundingBox() const { return FormBbox; }
	void SetFormationBoundingBox(FVector2D formBbox) { FormBbox = formBbox; }

	void SetSoldierMovementSpeed(float speed);
	void SetSoldierTargetLocationAndOrientation(const FVector& location, const FRotator& rotation);

	void AssignSoldierOffset(FRotator orientation) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class AFormationCommander* commander;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<class UFormationDescription> _formDescrRef;

	FVector2D FormBbox;
	TArray<FVector> FormationPositions; // describes the current relative offset for each soldier 
	int currentWidth, currentHeight;    // describes the current widht / height (num soldiers) of the formation
	
	// Actor object references
	TSubclassOf<class ASoldier> SoldierRef;
	TSubclassOf<class AFormationCommander> CommanderRef;
};
