// Fill out your copyright notice in the Description page of Project Settings.


#include "FormationFrame.h"
#include "FormationCommander.h"
#include "FormationDescription.h"

#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"


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
	FrameCollider->Deactivate();
}

// Called every frame
void AFormationFrame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFormationFrame::HandleWallOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Enter wall"));
	positionValid = false;
}

void AFormationFrame::HandleWallExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Exit wall"));
	positionValid = true;
}

void AFormationFrame::Init(const FVector& begin, const FVector& end, const TArray<AFormationCommander*> forms)
{
	FrameCollider->Activate();

	// <<<<<< TODO >>>>>>>
	// * init decal textures

	activeFormations = forms;

	// initialize output data
	targetLocations.Empty();
	targetSoldierLocations.Empty();
	formBboxes.Empty();
	targetLocations.SetNum(activeFormations.Num());
	targetSoldierLocations.SetNum(activeFormations.Num());
	formBboxes.SetNum(activeFormations.Num());


	frameBegin = begin;
	Update(end);

	SetActorLocation(begin);
}

void AFormationFrame::Update(const FVector& pos)
{
	FVector posDiff = (pos - frameBegin);
	float dragWidth = posDiff.Size();

	//float minWidth = 0, maxWidth = 0;
	float totalWidth = 0.f;
	float totalLength = 0.f;

	// clamp formation width to min / max widths
	int numForms = (int)activeFormations.Num();
	float spacePerFormation = dragWidth / (float)numForms;
	//float spaceLeft = dragWidth;

	for(int i = 0; i < numForms; i++)
	{
		AFormationCommander* comm = activeFormations[i];
		UFormationDescription* descr = comm->GetFormationDescription();

		//float availableSpace = spaceLeft / (numForms - i);
		
		targetSoldierLocations[i] = descr->GetFormationFromWidth(spacePerFormation, comm->GetNumSoldiers(), formBboxes[i]);

		// update bounding box properties
		//spaceLeft -= formBboxes[i].Y;
		totalLength = formBboxes[i].X > totalLength ? formBboxes[i].X : totalLength;
		totalWidth += formBboxes[i].Y;

		if ((i + 1) < numForms) totalWidth += formationPadding;
	}

	// set size
	float YExtent = totalWidth * 0.5f;
	float XExtent = totalLength * 0.5f;

	FVector extent(XExtent, YExtent, frameHeight);
	FrameCollider->SetBoxExtent(extent, true);

	// set location
	posDiff.Normalize();
	FVector relLoc = posDiff * YExtent;
	FrameCollider->SetRelativeLocation(FVector(relLoc.X, relLoc.Y, extent.Z * .5f));
	
	// set rotation
	FVector right = posDiff;
	FRotator rot = UKismetMathLibrary::MakeRotFromYZ(right, FVector(0.f, 0.f, 1.f));
	FrameCollider->SetRelativeRotation(rot.Quaternion());

	// update output data
	FVector rightVec = FrameCollider->GetRightVector();
	FVector leftEdge = FrameCollider->GetComponentLocation() - YExtent * rightVec;
	for (int i = 0; i < numForms; i++)
	{
		FVector pos = leftEdge + rightVec * formBboxes[i].Y * 0.5f;
		targetLocations[i] = pos;

		leftEdge += rightVec * (formBboxes[i].Y + formationPadding);
	}

	// update ouptut rotation
	targetRotation = rot;
}

void AFormationFrame::Stop(bool cancel)
{
	//UE_LOG(LogTemp, Warning, TEXT("Stop frame"));

	if (cancel)
	{
		return;
	}
	else
	{
		// set actual formation positions for this formation
		// <<<<< TODO >>>>>
		// basically only: 
		// * make sure commander gets assigned correct location (and not the one of some other commander)

		// inform commanders about target location and corresponding relative soldier positions
		int numFormations = activeFormations.Num();
		for (int i = 0; i < numFormations; i++)
		{
			activeFormations[i]->SetTargetLocation(targetLocations[i]);
			activeFormations[i]->SetFormationPositions(targetSoldierLocations[i]);
			activeFormations[i]->SetTargetRotation(targetRotation);
			activeFormations[i]->SetFormationBoundingBox(formBboxes[i]);
			//UE_LOG(LogTemp, Warning, TEXT("[%d] target location: x: %f, y: %f"), i, targetLocations[i].X, targetLocations[i].Y);
		}
	}

	FrameCollider->Deactivate();
}
