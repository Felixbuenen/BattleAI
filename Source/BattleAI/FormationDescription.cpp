// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationDescription.h"

TArray<FVector2D> UFormationDescription::GetFormationFromWidth(float width, int numSoldiers, FVector2D& outBBox)
{
	TArray<FVector2D> formation;

	int maxWidth = numSoldiers / minHeight;
	int numPerRow;

	// calculate the width of the formation
	if (width < (minWidth * agentClearance)) numPerRow = minWidth;
	else if (width > (maxWidth * agentClearance)) numPerRow = maxWidth;
	else
	{
		numPerRow = width / agentClearance;
	}

	int numPerCol = numSoldiers / numPerRow;
	int remaining = numSoldiers % numPerRow;

	// fill the complete rows
	for (int y = 0; y < numPerCol; y++)
	{
		for (int x = 0; x < numPerRow; x++)
		{
			formation.Emplace(FVector2D(x * agentClearance, y * agentClearance));
		}
	}

	// fill the last (incomplete) row
	// calculate left most agent x-position of the agents in the back row and add to formation
	float startPosX = 0.5f * (numPerRow - remaining) * agentClearance;
	for (int r = 0; r < remaining; r++)
	{
		formation.Emplace(FVector2D(startPosX + r * agentClearance, numPerCol * agentClearance));
	}

	outBBox.X = numPerRow * agentClearance;
	outBBox.Y = numPerCol * agentClearance;
	if (remaining > 0) outBBox.Y += agentClearance; // add one extra clearance value if the formation had remaining agents

	return formation;
}