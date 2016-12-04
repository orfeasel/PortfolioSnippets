// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "ScriptedDoor.h"

void AScriptedDoor::Rotate(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
}

// Sets default values
AScriptedDoor::AScriptedDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DoorSM=CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorSM"));

	SetRootComponent(DoorSM);
	
	TimelineHandleComp = CreateDefaultSubobject<UTimelineHandleComponent>(FName("TimelineHandleComp"));
}

// Called when the game starts or when spawned
void AScriptedDoor::BeginPlay()
{
	Super::BeginPlay();
	
	TimelineHandleComp->BindFloatCurveRotatorExecutionFunction(this, FName("Rotate"));
}

void AScriptedDoor::LockDoor()
{
	FRotator TargetRotation = GetActorRotation();
	TargetRotation.Yaw -= OpenYaw;

	TimelineHandleComp->SetInitialAndTargetValues(GetActorRotation(), TargetRotation);

	FOnTimelineEventStatic TimelineEvent;
	TimelineEvent.BindLambda([&]()
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DoorSlamSFX, GetActorLocation());
	});

	TimelineHandleComp->SetTimelineFinishedFunc(TimelineEvent);


	TimelineHandleComp->PlayTimeline();
}

