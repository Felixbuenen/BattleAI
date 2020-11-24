// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplicitCorridorMap.h"

bool UExplicitCorridorMap::FindPathWithClearance(FVector2D start, FVector2D goal, float clearance, const Corridor& corridor, std::vector<FVector2D>& _outPath) const
{
	return false;
}

bool UExplicitCorridorMap::FindCorridor(FVector2D start, FVector2D goal, Corridor& _outCorridor) const
{
	return false;
}

bool UExplicitCorridorMap::FindRetractionPoint(FVector2D source, Retraction& _outRetraction) const
{
	// first find the retraction edge: the ECM edge on which the retraction point will be located
	ECMEdge retrEdge;
	bool onLeftSide;
	bool foundEdge = GetRetractionEdge(source, retrEdge, onLeftSide);
	if (!foundEdge)
	{
		return false;
	}

	// found ECM edge where we should project our retraction point. Now find the actual retraction point + closest left & right points.
	FVector2D retrLocation = GetClosestPointToLineSegment(source, retrEdge.begin->location, retrEdge.end->location);
	_outRetraction.location = retrLocation;
	_outRetraction.nearest_left = GetClosestPointToLineSegment(retrLocation, retrEdge.begin->nearest_left, retrEdge.end->nearest_left);
	_outRetraction.nearest_right = GetClosestPointToLineSegment(retrLocation, retrEdge.begin->nearest_right, retrEdge.end->nearest_right);
	_outRetraction.clearance = retrEdge.begin->clearance;

	return true;
}

// Calculates if a point is inside a convex polygon. Points are expected to be be given in (anti-)clockwise order.
bool UExplicitCorridorMap::IsPointInsideConvexPolygon(const FVector2D& point, const std::vector<FVector2D>& cellPoints) const
{
	int numPoints = (int)cellPoints.size();
	if (numPoints < 3) return false;

	// check at which side the point lies of the first cell edge
	FVector2D firstEdge = cellPoints[0] - cellPoints[1];
	FVector2D affinePoint = point - cellPoints[1];
	bool firstSignNegative = FVector2D::CrossProduct(affinePoint, firstEdge) < 0.0f;

	// then, for the remaining line segments, check if the point lies on the same side by comparing the sign of the cross product (should be equal)
	for (int i = 1; i < numPoints; ++i)
	{
		FVector2D edge = cellPoints[i] - cellPoints[(i + 1) % numPoints];
		FVector2D affinePoint = point - cellPoints[(i + 1) % numPoints];
		bool signNegative = FVector2D::CrossProduct(affinePoint, firstEdge) < 0.0f;

		if (signNegative != firstSignNegative) return false;
	}

	return true;
}

bool UExplicitCorridorMap::GetRetractionEdge(FVector2D source, ECMEdge& _outEdge, bool& _outOnLeftSide) const
{
	std::vector<FVector2D> cellPointBuffer;
	cellPointBuffer.resize(4);

	for (const ECMEdge& edge : _edges)
	{
		// first check if point in left side of the cell...
		cellPointBuffer[0] = edge.begin->location; cellPointBuffer[1] = edge.begin->nearest_left;
		cellPointBuffer[2] = edge.end->nearest_left; cellPointBuffer[3] = edge.end->location;
		if (IsPointInsideConvexPolygon(source, cellPointBuffer))
		{
			// found the correct edge!
			_outEdge = edge;
			_outOnLeftSide = true;
			return true;
		}

		// ... then check if point in right side of the cell
		cellPointBuffer[0] = edge.begin->location; cellPointBuffer[1] = edge.end->location;
		cellPointBuffer[2] = edge.end->nearest_right; cellPointBuffer[3] = edge.begin->nearest_right;
		if (IsPointInsideConvexPolygon(source, cellPointBuffer))
		{
			// found the correct edge!
			_outEdge = edge;
			_outOnLeftSide = false;
			return true;
		}
	}

	return false;
}

FVector2D UExplicitCorridorMap::GetClosestPointToLineSegment(const FVector2D& point, const FVector2D& edgeBegin, const FVector2D& edgeEnd) const
{
	FVector2D beginToPoint = point - edgeBegin;
	FVector2D lineSeg = edgeEnd - edgeBegin;

	float magnitude = lineSeg.SizeSquared();    
	float dot = FVector2D::DotProduct(beginToPoint, lineSeg);
	float distance = dot / magnitude;

	if (distance < 0) // point is in front of line segment: return begin point     
	{
		return edgeBegin;
	}
	else if (distance > 1) // point is behind line segment: return end point
	{
		return edgeEnd;
	}
	else
	{
		return edgeBegin + lineSeg * distance;
	}
}