// Fill out your copyright notice in the Description page of Project Settings.


#include "PathPlanner.h"

#include "GlobalPath.h"
#include "Formation.h"

void UPathPlanner::Initialize(class AActor* terrain, const TSubclassOf<AActor>& obstacle)
{
	
}

AGlobalPath* UPathPlanner::FindPath(const AFormation* formation, const FVector& goal) const
{
	return nullptr;
}
