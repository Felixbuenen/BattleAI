// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FormationFrame.generated.h"

UCLASS()
class BATTLEAI_API AFormationFrame : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFormationFrame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* FramePivot;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* FrameCollider;

	UFUNCTION()
	void HandleWallOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const struct FHitResult& SweepResult);
	UFUNCTION()
	void HandleWallExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	const float frameHeight = 64.f;
	const float formationPadding = 100.f;
	FVector frameBegin;
	TArray<class AFormationCommander*> activeFormations;

	// todo: make struct
	TArray<FVector> targetLocations;
	TArray<TArray<FVector>> targetSoldierLocations;
	FRotator targetRotation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Init(const FVector& begin, const FVector& end, const TArray<AFormationCommander*> forms);
	
	UFUNCTION(BlueprintCallable)
	void Update(const FVector& pos);

	UFUNCTION(BlueprintCallable)
	void Stop(bool cancel);
};