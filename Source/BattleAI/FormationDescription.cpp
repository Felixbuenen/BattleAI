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
	float remainderClearance = remaining > 0 ? agentClearance : 0.f; // add one extra clearance value if the formation had remaining agents

	outBBox.Y = numPerRow * agentClearance;
	outBBox.X = numPerCol * agentClearance + remainderClearance;

	float startY = -1 * (outBBox.Y * 0.5f) + (agentClearance * 0.5f);
	float startX = -1 * (outBBox.X * 0.5f) + (agentClearance * 0.5f) + remainderClearance;

	int counter = 0;

	// fill the complete rows
	for (int y = 0; y < numPerRow; y++)
	{
		for (int x = numPerCol - 1; x >= 0; x--)
		{
			formation.Emplace(FVector(startX + x * agentClearance, startY + y * agentClearance, 0.f));
			counter++;
		}
	}

	// fill the last (incomplete) row
	// calculate left most agent y-position of the agents in the back row and add to formation
	startY = startY + (numPerRow - remaining) * 0.5f * agentClearance;
	for (int r = 0; r < remaining; r++)
	{
		formation.Emplace(FVector(startX - remainderClearance, startY + r * agentClearance, 0.f));
		counter++;
	}

	//UE_LOG(LogTemp, Warning, TEXT("Num soldiers: %d, number of positions: %d. rows: %d, cols: %d, remain: %d"), numSoldiers, counter, numPerRow, numPerCol, remaining);

	return formation;
}