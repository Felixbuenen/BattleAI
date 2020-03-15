// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationDescription.h"

TArray<FVector> UFormationDescription::GetFormationFromWidth(float width, int numSoldiers, FVector2D& outBBox)
{
	TArray<FVector> formation;

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
	outBBox.Y = numPerRow * agentClearance;
	outBBox.X = numPerCol * agentClearance;
	if (remaining > 0) outBBox.X += agentClearance; // add one extra clearance value if the formation had remaining agents

	float halfWidth = outBBox.Y * 0.5f;
	float halfHeight = outBBox.X * 0.5f;

	// fill the complete rows
	for (int y = 0; y < numPerRow; y++)
	{
		for (int x = 0; x < numPerCol; x++)
		{
			formation.Emplace(FVector(x * agentClearance - halfHeight, y * agentClearance - halfWidth, 0.f));
		}
	}

	// fill the last (incomplete) row
	// calculate left most agent y-position of the agents in the back row and add to formation
	float startPosY = 0.5f * (numPerRow - remaining) * agentClearance - halfWidth;
	for (int r = 0; r < remaining; r++)
	{
		formation.Emplace(FVector(numPerCol * agentClearance - halfHeight, startPosY + r * agentClearance, 0.f));
	}



	return formation;
}