// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MoTPianoLid.h"




void AMoTPianoLid::Rotate(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
}

AMoTPianoLid::AMoTPianoLid()
{
	TimelineHandleComponent = CreateDefaultSubobject<UTimelineHandleComponent>(FName("TimelineHandleComponent"));
}

void AMoTPianoLid::BeginPlay()
{
	Super::BeginPlay();

	FRotator TargetRotation = GetActorRotation();
	TargetRotation.Roll -= 100.f;

	TimelineHandleComponent->SetInitialAndTargetValues(GetActorRotation(), TargetRotation);

	TimelineHandleComponent->BindFloatCurveRotatorExecutionFunction(this, FName("Rotate"));

	FOnTimelineEventStatic TimelineFinishedFunction;
	TimelineFinishedFunction.BindLambda([&]()
	{
		bIsOpen = true;
	});

	TimelineHandleComponent->SetTimelineFinishedFunc(TimelineFinishedFunction);
}

bool AMoTPianoLid::Interact(UBackpackComponent& BackpackComponent)
{
	if (Super::Interact(BackpackComponent) && !bIsOpen)
	{
		TimelineHandleComponent->PlayTimeline();
		return true;
	}
	return false;
}
