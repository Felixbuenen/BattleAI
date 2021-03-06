// Fill out your copyright notice in the Description page of Project Settings.


#include "Soldier.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASoldier::ASoldier()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;

	
}

// Called when the game starts or when spawned
void ASoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

