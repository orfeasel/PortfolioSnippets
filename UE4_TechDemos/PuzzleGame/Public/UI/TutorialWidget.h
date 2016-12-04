// Orfeas Eleftheriou 2016

#pragma once

#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	/** Marked as BlueprintReadOnly in order to bind a text block to this*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString DisplayingMessage;

public:

	FORCEINLINE void SetDisplayMessage(FString MessageToDisplay)
	{
		DisplayingMessage = MessageToDisplay;
	}
	
};
