// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationFrame.h"
#include "FormationCommander.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"


// Sets default values
AFormationFrame::AFormationFrame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FramePivot = CreateDefaultSubobject<USceneComponent>(TEXT("Frame Pivot"));
	FrameCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Frame Collider"));
	
	FrameCollider->SetHiddenInGame(false); // for now, make it visible
	FrameCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FrameCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	FrameCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	// NOT SURE WHAT "WALL" PROFILE IS
	//FrameCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	FrameCollider->SetGenerateOverlapEvents(true);
	FrameCollider->SetBoxExtent(FVector(1.f, 1.f, frameHeight));
	FrameCollider->SetRelativeLocation(FVector(0.f, 0.f, frameHeight / 2.f));

	SetRootComponent(FramePivot);
	FrameCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFormationFrame::BeginPlay()
{
	Super::BeginPlay();
	
	FrameCollider->OnComponentBeginOverlap.AddDynamic(this, &AFormationFrame::HandleWallOverlap);
	FrameCollider->OnComponentEndOverlap.AddDynamic(this, &AFormationFrame::HandleWallExit);
	//FrameCollider->Deactivate();
}

// Called every frame
void AFormationFrame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFormationFrame::HandleWallOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter wall"));
}

void AFormationFrame::HandleWallExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit wall"));
}

void AFormationFrame::Init(const FVector& begin, const FVector& end, const TArray<AFormationCommander*> forms)
{
	UE_LOG(LogTemp, Warning, TEXT("Init frame"));

	SetActorLocation(begin);
	frameBegin = begin;
	Update(end);

	activeFormations = forms;
}

void AFormationFrame::Update(const FVector& pos)
{
	//UE_LOG(LogTemp, Warning, TEXT("Updating frame..."));

	FVector extent = (pos - frameBegin) * 0.5f;
	extent.Z = frameHeight;
	FrameCollider->SetRelativeLocation(FVector(extent.X, extent.Y, frameHeight / 2.f));
	FrameCollider->SetBoxExtent(extent, true);

	//DrawDebugSolidBox(GetWorld(), FrameCollider->GetComponentLocation(), FrameCollider->GetScaledBoxExtent(), FColor::Red, false);
}

void AFormationFrame::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop frame"));

	//FrameCollider->Deactivate();
}
