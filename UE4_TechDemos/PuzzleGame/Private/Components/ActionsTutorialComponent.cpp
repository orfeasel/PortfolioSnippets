// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "ActionsTutorialComponent.h"

void UActionsTutorialComponent::EraseDisplayedMessage(float Delay)
{
	FTimerHandle TimerHandle;

	FTimerDelegate TimerDelegate;

	TimerDelegate.BindLambda([&]()
	{
		TutorialWidget->SetDisplayMessage(FString());
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, TutorialMessageDisplaySeconds, false);
}

FString UActionsTutorialComponent::GetTutorialMessageBasedOnType(ETutorialMessageType TutorialType)
{
	switch (TutorialType)
	{
		case ETutorialMessageType::EMovementTutorial: return MovementTutorialMessage;

		case ETutorialMessageType::EUseItemTutorial: return UseItemTutorialMessage;

		case ETutorialMessageType::EPickupTutorial: return PickupedUpItemTutorialMessage;

		case ETutorialMessageType::EGameFinish: return DemoFinishMessage;

		default:
			return FString();
	}
}

void UActionsTutorialComponent::InitializeTutorialMessagesMap()
{
	DisplayedMessages.Add(ETutorialMessageType::EMovementTutorial, false);
	DisplayedMessages.Add(ETutorialMessageType::EUseItemTutorial, false);
	DisplayedMessages.Add(ETutorialMessageType::EPickupTutorial, false);
	DisplayedMessages.Add(ETutorialMessageType::EGameFinish, false);
}

// Sets default values for this component's properties
UActionsTutorialComponent::UActionsTutorialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UActionsTutorialComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeTutorialMessagesMap();
}

void UActionsTutorialComponent::DisplayTutorialMessageBasedOnSeeingItem(ABaseItem* SeeingItem)
{
	if (!SeeingItem) return;
	bool* DisplayedUseItemTutorial = DisplayedMessages.Find(ETutorialMessageType::EUseItemTutorial);

	if (DisplayedUseItemTutorial && !*DisplayedUseItemTutorial)
	{
		TutorialWidget->SetDisplayMessage(UseItemTutorialMessage);

		EraseDisplayedMessage(TutorialMessageDisplaySeconds);

		//Mark this tutorial as displayed
		*DisplayedUseItemTutorial = true;
	}
}

void UActionsTutorialComponent::DisplayTutorialMessage(ETutorialMessageType MessageToDisplay)
{
	bool* HasDisplayedThisTutorial = DisplayedMessages.Find(MessageToDisplay);

	if (HasDisplayedThisTutorial && !*HasDisplayedThisTutorial)
	{
		TutorialWidget->SetDisplayMessage(GetTutorialMessageBasedOnType(MessageToDisplay));

		EraseDisplayedMessage(TutorialMessageDisplaySeconds);

		//Mark this tutorial as displayed
		*HasDisplayedThisTutorial = true;
	}
}

void UActionsTutorialComponent::EraseDisplayingMessage()
{
	TutorialWidget->SetDisplayMessage(FString());

	EraseDisplayedMessage(0.f);
}

