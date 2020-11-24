// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathPlanner.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAI_API UPathPlanner : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void Initialize(class AActor* terrain, const TSubclassOf<AActor>& obstacle);
	
	UFUNCTION(BlueprintCallable)
	virtual class AGlobalPath* FindPath(const class AFormation* formation, const FVector& goal) const;

	void DrawDebugInfo(int infoKey) const;

protected:
	class UDebugRenderer* _debugRenderer;
};
