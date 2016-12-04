// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "IntroBoat.h"
#include "Public/Character/MainCharacter.h"
#include "Public/Components/ActionsTutorialComponent.h"
#include "LevelScripts/MainMapLevelScript.h"

void AIntroBoat::MoveToShore(FVector NewLocation)
{
	SetActorLocation(NewLocation);
}

void AIntroBoat::OnShoreArrived()
{
	//Disable all the blocking colliders
	LeftBoxComp->DestroyComponent();
	RightBoxComp->DestroyComponent();
	RearBoxComp->DestroyComponent();
	FrontBoxComp->DestroyComponent();

	//Enable the AI
	if (ButtlerCharacterRef) ButtlerCharacterRef->WalkToCabin();

	AMainCharacter* MainChar = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	MainChar->ActivateBackpackupInput();
	MainChar->ActivateJumpInput();

	//Display the movement tutorial
	MainChar->GetActionsTutorialComponent()->DisplayTutorialMessage(ETutorialMessageType::EMovementTutorial);

	//Enable the collider on the port so the player will be restricted in the demo prepared level only
	AMainMapLevelScript* LevelScript = Cast<AMainMapLevelScript>(GetWorld()->GetLevelScriptActor());

	if (LevelScript) LevelScript->ActivateEntryDockCollider();
}

// Sets default values
AIntroBoat::AIntroBoat()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoatSM = CreateDefaultSubobject<UStaticMeshComponent>(FName("BoatSM"));

	SetRootComponent(BoatSM);

	LeftBoxComp = CreateDefaultSubobject<UBoxComponent>(FName("LeftBoxComp"));
	RightBoxComp = CreateDefaultSubobject<UBoxComponent>(FName("RightBoxComp"));
	RearBoxComp = CreateDefaultSubobject<UBoxComponent>(FName("RearBoxComp"));
	FrontBoxComp = CreateDefaultSubobject<UBoxComponent>(FName("FrontBoxComp"));

	LeftBoxComp->SetupAttachment(BoatSM);
	RightBoxComp->SetupAttachment(BoatSM);
	RearBoxComp->SetupAttachment(BoatSM);
	FrontBoxComp->SetupAttachment(BoatSM);

	TimelineHandleComp = CreateDefaultSubobject<UTimelineHandleComponent>(FName("TimelineHandleComp"));
}

// Called when the game starts or when spawned
void AIntroBoat::BeginPlay()
{
	Super::BeginPlay();

	TimelineHandleComp->SetInitialAndTargetValues(GetActorLocation(), TargetLocation);

	TimelineHandleComp->BindFloatCurveVectorExecutionFunction(this, FName("MoveToShore"));

	FOnTimelineEventStatic TimelineFinishEvent;

	TimelineFinishEvent.BindUFunction(this, FName("OnShoreArrived"));

	TimelineHandleComp->SetTimelineFinishedFunc(TimelineFinishEvent);

	TimelineHandleComp->PlayTimeline();
}

