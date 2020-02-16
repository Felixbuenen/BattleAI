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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void HandleLeftMouseDown(class AFormationCommander* formation);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HandleRightMouseDown();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void HandleRightMouseUp();

	UPROPERTY(BlueprintReadOnly)
	TArray<class AFormationCommander*> activeFormations;
	
	void ToggleSelectFormation(bool selected, AFormationCommander* formation, int index = -1);
	void DeselectAllFormations();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AFormationFrame* frame;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
