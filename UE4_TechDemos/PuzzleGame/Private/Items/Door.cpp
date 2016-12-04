// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "Door.h"
#include "Components/BackpackComponent.h"



void ADoor::Rotate(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
}

void ADoor::ToggleDoorOpenStatus()
{
	bIsClosed = !bIsClosed;
}

ADoor::ADoor()
{
	TimelineHandleComp = CreateDefaultSubobject<UTimelineHandleComponent>(FName("TimelineHandleComp"));
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	TimelineHandleComp->BindFloatCurveRotatorExecutionFunction(this, FName("Rotate"));
	
	TimelineHandleComp->BindTimelineFinishedFunc(this, FName("ToggleDoorOpenStatus"));

	FRotator TargetRotation = FRotator(0, MaxOpenYawValue, 0);
	TargetRotation += GetActorRotation();

	TimelineHandleComp->SetInitialAndTargetValues(GetActorRotation(), TargetRotation);
}

bool ADoor::Interact(UBackpackComponent& BackpackComponent)
{
	if (Super::Interact(BackpackComponent) && !TimelineHandleComp->IsPlayingATimeline())
	{
		if (bIsClosed)
		{
			TimelineHandleComp->PlayTimeline();
		}
		else
		{
			TimelineHandleComp->ReverseTimelineFromEnd();
		}
		return true;
	}
	return false;
}
