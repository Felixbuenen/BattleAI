// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AStarSolver.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalPath.generated.h"

/**
 *
 */
UCLASS()
class BATTLEAI_API AGlobalPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlobalPath();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class USplineComponent* splinePath;

	std::vector<NodePosition> pathPositions;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitPath(std::vector<NodePosition>& pathPoints);

	FRotator GetDirectionAtPercentile(float percentile) const;
	FVector GetLocationAtPercentile(float percentile) const;

	float GetPathLength() const;

	//virtual FVector GetAttractionPoint(const FVector& location) const = 0;
	//virtual FRotator GetEndpointRotation() const = 0;
	//virtual bool HasReachedDestination() const = 0;
};
