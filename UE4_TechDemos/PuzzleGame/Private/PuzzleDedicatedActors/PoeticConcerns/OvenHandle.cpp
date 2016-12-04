// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "OvenHandle.h"
#include "DrawDebugHelpers.h"
#include "PCOven.h"

void AOvenHandle::TimelineExecutionFunction(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
}

void AOvenHandle::TimelineFinishFunction()
{
	TimelineHandleComponent->SetInitialAndTargetValues(GetActorRotation(), GetActorRotation() + AddedRotator);

	FacingDirection = MathFunctions::CalculateDirection(GetActorRotation());

	PCOvenReference->CheckWinCondition();
}

AOvenHandle::AOvenHandle()
{
	TimelineHandleComponent = CreateDefaultSubobject<UTimelineHandleComponent>(FName("TimelineHandleComponent"));
}

void AOvenHandle::BeginPlay()
{
	Super::BeginPlay();

	FacingDirection = MathFunctions::CalculateDirection(GetActorRotation());

	TimelineHandleComponent->SetInitialAndTargetValues(GetActorRotation(), GetActorRotation() + AddedRotator);

	TimelineHandleComponent->BindFloatCurveRotatorExecutionFunction(this, FName("TimelineExecutionFunction"));

	FOnTimelineEventStatic TimelineFinishFunc;
	TimelineFinishFunc.BindUFunction(this, FName("TimelineFinishFunction"));

	TimelineHandleComponent->SetTimelineFinishedFunc(TimelineFinishFunc);

	PCOvenReference = Cast<APCOven>(GetAttachParentActor());
	PCOvenReference->RegisterOvenHandle(this);

}

void AOvenHandle::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//todo: remove dis.
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorRightVector() * 200.f, FColor::Blue);
}

bool AOvenHandle::Interact(UBackpackComponent& BackpackComponent)
{
	Super::Interact(BackpackComponent);

	TimelineHandleComponent->PlayTimeline();

	return true;
}
