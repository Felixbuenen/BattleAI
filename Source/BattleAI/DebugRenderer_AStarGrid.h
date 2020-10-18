// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DebugRenderer.h"

#include <map>

#include "DebugRenderer_AStarGrid.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEAI_API UDebugRenderer_AStarGrid : public UDebugRenderer
{
	GENERATED_BODY()
	
	typedef void (UDebugRenderer_AStarGrid::*DrawDelegate)(void) const;

public:
	virtual void ToggleRender(int debugKey) const override;

protected:
	virtual void SetupRenderDelegates() override;

private:
	// RENDER DELEGATES
	void DrawGrid() const;
	void DrawGridPaths() const;

	std::map<int, DrawDelegate> drawDelegates;

	mutable bool activeDrawGrid;
	mutable bool activeDrawGridPath;
};
