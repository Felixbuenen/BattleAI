// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugRenderer_AStarGrid.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "PathPlanner_AStarGrid.h"
#include "DebugRenderer.h"
#include "AStarSolver.h"
#include "GlobalPath.h"

void UDebugRenderer_AStarGrid::ToggleRender(int debugKey) const
{
	// first check if the delegate key exists
	auto drawDelegateIt = drawDelegates.find(debugKey);
	if (drawDelegateIt == drawDelegates.end()) return;

	// if it does, call the delegate
	DrawDelegate drawDelegate = drawDelegateIt->second;
	(this->*drawDelegate)();
}

void UDebugRenderer_AStarGrid::SetupRenderDelegates()
{
	// setup delegates, for now just assign simple hard-coded input keys

	drawDelegates.emplace(std::make_pair<int, DrawDelegate>(1, &UDebugRenderer_AStarGrid::DrawGrid));
}

void UDebugRenderer_AStarGrid::DrawGrid() const
{
	if (activeDrawGrid)
	{
		activeDrawGrid = false;
		FlushPersistentDebugLines(_worldRef);
		return;
	}

	activeDrawGrid = true;

	UE_LOG(LogTemp, Warning, TEXT("DRAW GRID"));
	UPathPlanner_AStarGrid* gridPlannerRef = static_cast<UPathPlanner_AStarGrid*>(_plannerRef);
	if (gridPlannerRef == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("cannot draw grid: planner reference is nullptr"));
		return;
	}
	UAStarSolver* solver = gridPlannerRef->solver;
	if (solver == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("cannot draw grid: solver is nullptr"));
		return;
	}
	int gridWidth, gridHeight;
	solver->GetGridDimensions(gridWidth, gridHeight);
	float cellExtent = gridPlannerRef->cellSize * 0.5f;
	UE_LOG(LogTemp, Warning, TEXT("width: %d, height: %d"), gridWidth, gridHeight);

	FlushPersistentDebugLines(_worldRef);
	for (int x = 0; x < gridWidth; x++)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			//FVector center(x * CellExtent * 2 - levelBoundX + CellExtent, y * CellExtent * 2 - levelBoundY + CellExtent, 70);
			FVector center = solver->GetPosition(x + y * gridWidth);
			center.Z += 100.f;
			FVector extend(cellExtent);

			int clearance = solver->GetClearance(x + y * gridWidth);

			const int maxSearchDepth = 20;
			FColor clr = FColor::MakeRedToGreenColorFromScalar(clearance / (float)(maxSearchDepth));

			//UE_LOG(LogTemp, Warning, TEXT("drawing at pos: %s, with extent: %s, and clearance: %d"), *center.ToString(), *extend.ToString(), clearance);
			if (clearance == 0)
			{
				DrawDebugBox(_worldRef, center, extend, FColor::Red, true);
			}
			else
			{
				DrawDebugBox(_worldRef, center, extend, clr, true);
			}
		}
	}
}

void UDebugRenderer_AStarGrid::DrawGridPaths() const
{
	TArray<AActor*> outPaths;
	UGameplayStatics::GetAllActorsOfClass((const UObject*)_worldRef, AGlobalPath::StaticClass(), outPaths);

	for(AActor* pathActor : outPaths)
	{
		AGlobalPath* path = Cast<AGlobalPath>(pathActor);

		UPathPlanner_AStarGrid* gridPlannerRef = static_cast<UPathPlanner_AStarGrid*>(_plannerRef);
		if (gridPlannerRef == nullptr) return;

		float cellExtent = gridPlannerRef->cellSize * 0.5f;
		const std::vector<NodePosition>& positions = path->GetPathPositions();
		for (const NodePosition& pos : positions)
		{
			FVector center(pos.x, pos.y, 70);
			FVector extend(cellExtent);
			DrawDebugSolidBox(GetWorld(), center, extend, FColor::Red, false);
		}
	}
}
