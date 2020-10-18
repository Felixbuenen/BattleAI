// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <map>

#include "DebugRenderer.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAI_API UDebugRenderer : public UObject
{
	GENERATED_BODY()

public:

	static UDebugRenderer* GetNewDebugRenderer(class UPathPlanner* planner);

	void Initialize(class UWorld* worldRef, class UPathPlanner* plannerRef);
	virtual void ToggleRender(int debugKey) const;

protected:
	virtual void SetupRenderDelegates();

	class UPathPlanner* _plannerRef;
	class UWorld* _worldRef;
};
