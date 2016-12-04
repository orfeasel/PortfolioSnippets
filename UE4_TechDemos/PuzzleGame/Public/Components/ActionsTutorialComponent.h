// Orfeas Eleftheriou 2016

#pragma once

#include "Components/ActorComponent.h"
#include "UI/TutorialWidget.h"
#include "ActionsTutorialComponent.generated.h"

class ABaseItem;

UENUM(BlueprintType)
enum class ETutorialMessageType : uint8
{
	EMovementTutorial,
	EUseItemTutorial,
	EPickupTutorial,
	EGameFinish
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNA_API UActionsTutorialComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	UTutorialWidget* TutorialWidget;

	TMap<ETutorialMessageType, bool> DisplayedMessages;

	void EraseDisplayedMessage(float Delay);

	FString GetTutorialMessageBasedOnType(ETutorialMessageType TutorialType);

	void InitializeTutorialMessagesMap();

protected:

	UPROPERTY(EditDefaultsOnly, Category = TutorialMessages)
	float TutorialMessageDisplaySeconds;

	UPROPERTY(EditDefaultsOnly, Category = TutorialMessages)
	FString MovementTutorialMessage;

	UPROPERTY(EditDefaultsOnly, Category = TutorialMessages)
	FString UseItemTutorialMessage;

	UPROPERTY(EditDefaultsOnly, Category = TutorialMessages)
	FString PickupedUpItemTutorialMessage;

	UPROPERTY(EditDefaultsOnly, Category = TutorialMessages)
	FString DemoFinishMessage;

public:	
	// Sets default values for this component's properties
	UActionsTutorialComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	FORCEINLINE void SetTutorialWidgetReference(UTutorialWidget* TutorialWidgetRef)
	{
		TutorialWidget = TutorialWidgetRef;
	}

	void DisplayTutorialMessageBasedOnSeeingItem(ABaseItem* SeeingItem);
	
	void DisplayTutorialMessage(ETutorialMessageType MessageToDisplay);

	void SetTutorialMessageDisplaySeconds(float MessageDuration)
	{
		TutorialMessageDisplaySeconds = MessageDuration;
	}

	void EraseDisplayingMessage();
};
