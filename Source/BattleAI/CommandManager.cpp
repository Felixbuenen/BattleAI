// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandManager.h"

#include "FormationCommander.h"

// Sets default values
ACommandManager::ACommandManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACommandManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommandManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACommandManager::HandleLeftMouseDown(AFormationCommander* formation)
{
	if (formation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Formation with %d soldiers"), (int)formation->Soldiers.Num());
	}
}

void ACommandManager::HandleRightMouseDown()
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Down"));
}

void ACommandManager::HandleRightMouseUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Mouse Up"));
}

void ACommandManager::HandleMouseMoved(FVector2D screenPos)
{

}
