// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MainCharacterController.h"
#include "Public/Components/ActionsTutorialComponent.h"
#include "Public/Character/MainCharacter.h"

void AMainCharacterController::SetMovementStatus(bool NewStatus)
{
	if (NewStatus)
	{
		GetCharacter()->GetMovementComponent()->Activate(true);
		GetCharacter()->GetCharacterMovement()->Activate(true);
	}
	else
	{
		GetCharacter()->GetMovementComponent()->Deactivate();
		GetCharacter()->GetCharacterMovement()->Deactivate();
	}	
}

void AMainCharacterController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	if (DocumentReadingWidgetBP)
	{
		DocumentReadingWidgetRef = CreateWidget<UDocumentReadingWidget>(this, DocumentReadingWidgetBP);
		DocumentReadingWidgetRef->AddToViewport();
	}

	if (BackpackWidgetBP)
	{
		//Create the widget, add it to viewport and collapse the backpack canvas panel via BP
		BackpackWidgetRef = CreateWidget<UBackpackWidget>(this, BackpackWidgetBP);
		BackpackWidgetRef->AddToViewport();
		BackpackWidgetRef->SetBackpackVisible(false);
	}

	if (TutorialWidgetBP)
	{
		UTutorialWidget* TutorialWidget = CreateWidget<UTutorialWidget>(this, TutorialWidgetBP);
		TutorialWidget->AddToViewport();
		
		(Cast<AMainCharacter>(InPawn))->GetActionsTutorialComponent()->SetTutorialWidgetReference(TutorialWidget);
	}

	if (PauseWidgetBP)
	{
		PauseMenuWidgetRef = CreateWidget<UPauseMenuWidget>(this, PauseWidgetBP);
		PauseMenuWidgetRef->AddToViewport();
	}
}

void AMainCharacterController::ReadDocument(ADocument* DocumentToRead)
{
	if (!DocumentToRead && !bIsReading) return;

	bIsReading = true;

	//Update the document's image and add it to viewport
	DocumentReadingWidgetRef->SetImageTexture(DocumentToRead->GetDocumentTexture());

	//Restrict movement and camera control
	SetMovementStatus(false);
}

void AMainCharacterController::CloseDocument()
{
	if (!DocumentReadingWidgetRef && bIsReading) return;

	bIsReading = false;

	DocumentReadingWidgetRef->SetImageTexture(nullptr);

	SetMovementStatus(true);
}

void AMainCharacterController::HandleBackpackInput()
{
	if (IsAbleToInteract())
	{
		SetMovementStatus(false);
		bShowMouseCursor = true;

		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);

		bIsBackpackOpen = true;

		BackpackWidgetRef->SetBackpackVisible(true);
	}
	else if(bIsBackpackOpen)
	{
		BackpackWidgetRef->SetBackpackVisible(false);

		SetMovementStatus(true);
		bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		bIsBackpackOpen = false;
	}
}

void AMainCharacterController::SetEquippedItem(ABaseItem* NewItem)
{
	BackpackWidgetRef->SetEquippedItemTexture(NewItem);
}

void AMainCharacterController::SetGamePausedStatus(bool NewPauseStatus)
{
	if (!PauseMenuWidgetRef) return;

	if (NewPauseStatus)
	{
		//PauseMenuWidgetRef->AddToViewport();

		bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		SetInputMode(InputMode);
	}
	else
	{
		//PauseMenuWidgetRef->RemoveFromViewport();

		bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);
	}
}
