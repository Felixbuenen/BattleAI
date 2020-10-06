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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool multiSelect;

	UPROPERTY(EditAnywhere)
	bool activeFormationFrame;
	
	UPROPERTY(EditAnywhere)
	bool formationFrameDragging;

	UPROPERTY(EditAnywhere)
	float screenDragFrameTriggerTreshold = 20.0f;
	FVector2D frameStartDragScreenPosition;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ToggleSelectFormation(class AFormationCommander* formation);

	UFUNCTION(BlueprintCallable)
	void StartDragFormationGoal();

	UFUNCTION(BlueprintCallable)
	bool ValidFormationPosition() const;

	UFUNCTION(BlueprintCallable)
	void StopDragFormationGoal();

	UPROPERTY(BlueprintReadOnly)
	TArray<class AFormationCommander*> activeFormations;
	
	void ToggleSelectFormation(bool selected, AFormationCommander* formation, int index = -1);
	void DeselectAllFormations();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AFormationFrame* frame;

	const FVector GetWorldCursorLocation() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
