// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/PlayerController.h"
#include "Public/UI/DocumentReadingWidget.h"
#include "Public/UI/BackpackWidget.h"
#include "Public/UI/TutorialWidget.h"
#include "Public/Items/Document.h"
#include "UI/PauseMenuWidget.h"
#include "MainCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API AMainCharacterController : public APlayerController
{
	GENERATED_BODY()

private:

	/** Document Widget Reference */
	UDocumentReadingWidget* DocumentReadingWidgetRef;

	/** True if the player is reading a document */
	bool bIsReading;

	/** Sets the movement and camera status
	* @param NewStatus - the new status of the movement and camera (a false value disables movement and camera handling)
	*/
	void SetMovementStatus(bool NewStatus);

	UBackpackWidget* BackpackWidgetRef;

	bool bIsBackpackOpen = false;

	UPauseMenuWidget* PauseMenuWidgetRef;

protected:

	/** UMG Document Widget Reference */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDocumentReadingWidget> DocumentReadingWidgetBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBackpackWidget> BackpackWidgetBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTutorialWidget> TutorialWidgetBP;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPauseMenuWidget> PauseWidgetBP;

public:

	/** Returns true if the player can look around */
	inline bool IsAbleToInteract()
	{
		return !bIsReading && !bIsBackpackOpen;
	}

	virtual void Possess(APawn* InPawn) override;

	
	/** Opens the given document on screen and disables the player movement and camera control
	* @param DocumentToRead - the document to add on screen
	*/
	void ReadDocument(ADocument* DocumentToRead);

	/** Closes the document that is currently occupying the screen */
	void CloseDocument();

	UFUNCTION()
	void HandleBackpackInput();

	void SetEquippedItem(ABaseItem* NewItem);

	FORCEINLINE bool IsReadingADocument() { return bIsReading; }

	UFUNCTION(BlueprintCallable, Category = MainCharacterController)
	void SetGamePausedStatus(bool NewPauseStatus);
};
