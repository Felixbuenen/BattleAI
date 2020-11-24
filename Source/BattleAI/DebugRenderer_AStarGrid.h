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
	virtual void Render(int debugKey) const override;
	
	virtual void DrawPaths() const override;

protected:
	virtual void SetupRenderDelegates() override;

private:
	// RENDER DELEGATES
	void DrawGrid() const;

	std::map<int, DrawDelegate> drawDelegates;
};
