// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
//#include "DrawDebugHelpers.h"
#include "Drawer.h"


DEFINE_LOG_CATEGORY_STATIC(Drawer, Log, All);

void ADrawer::TimelineExecutionFunction(FVector NewLocation)
{
	SetActorLocation(NewLocation);
}

ADrawer::ADrawer()
{
	TimelineHandleComp = CreateDefaultSubobject<UTimelineHandleComponent>(FName("TimelineHandleComponent"));
}

void ADrawer::BeginPlay()
{
	Super::BeginPlay();

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * DrawLength, FColor::Blue, true);

	//TimelineHandleComp->SetTargetVector(GetActorLocation() + /*GetActorForwardVector()*/ GetActorRightVector() *DrawLength);

	//Using GetActorRightVector instead of Forward because static mesh of the drawer object is by default rotated by 90 degrees
	TimelineHandleComp->SetInitialAndTargetValues(GetActorLocation(), GetActorLocation() + GetActorRightVector() * DrawLength);

	TimelineHandleComp->BindFloatCurveVectorExecutionFunction(this, FName("TimelineExecutionFunction"));

	FOnTimelineEventStatic TimelineFinishedFunc;
	TimelineFinishedFunc.BindLambda([&]()
	{
		//Swap the initial and target locations in order to avoid 
		//checking if we have to play the timeline from the start or reverse it from the end 
		//(i'm lazy)
		TimelineHandleComp->SwapValues(UTimelineHandleComponent::E_VectorValue);
	});

	TimelineHandleComp->SetTimelineFinishedFunc(TimelineFinishedFunc);
}

bool ADrawer::Interact(UBackpackComponent& BackpackComponent)
{
	bool InteractionResult = Super::Interact(BackpackComponent);

	TimelineHandleComp->PlayTimeline();

	return InteractionResult;
}

void ADrawer::PrintWarnings()
{
	Super::PrintWarnings();

	if (DrawLength == 0) UE_LOG(Drawer, Warning, TEXT("The draw length of %s equals zero - you won't be able to see any transformation from its timeline"), *GetNameSafe(this));
}
