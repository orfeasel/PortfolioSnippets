// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "Public/Controllers/RnCPlayerController.h"
#include "RnCCharacter.h"

void ARnCPlayerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	//Create instances of our blueprint UIs

	if (PauseMenuWidgetBP) PauseMenuWidget = CreateWidget<UPauseMenuWidget>(this, PauseMenuWidgetBP);

	if (MainCharWidgetBP)
	{
		MainCharWidget = CreateWidget<UMainCharUIWidget>(this, MainCharWidgetBP);

		MainCharWidget->AddToViewport();
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	bShowMouseCursor = false;
}

void ARnCPlayerController::UnPossess()
{
	Super::UnPossess();
	if (DeathMenuWidgetBP)
	{
		DeathMenuWidget = CreateWidget<UDeathMenuWidget>(this, DeathMenuWidgetBP);
		if (DeathMenuWidget)
		{
			DeathMenuWidget->Show();

			//DeathMenuWidget->AddToViewport();

			bShowMouseCursor = true;

			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
		}

	}
}

//-------------------------------------------------------------------------------------
//Responsible for pausing / un-pausing the game and handling the corresponding UI
//-------------------------------------------------------------------------------------
void ARnCPlayerController::HandlePauseState()
{
	if(GetWorld()->IsPaused())
	{
		//Hide the cursor
		bShowMouseCursor = false;

		if (PauseMenuWidget)
		{
			//Play the Hide Animation
			PauseMenuWidget->Hide();

			//Set the focus to our game screen
			FInputModeGameOnly InputMode;
			SetInputMode(InputMode);

			//Unpause the game
			SetPause(false);
		}

	}
	else
	{
		//Show the cursor
		bShowMouseCursor = true;

		if (PauseMenuWidget)
		{
			//Play the show animation and load the widget to our screen
			PauseMenuWidget->Show();

			//Set the focus to our UI
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(PauseMenuWidget->TakeWidget());
			SetInputMode(InputMode);

			//Pause the Game
			SetPause(true);
		}

	}
}

void ARnCPlayerController::EnableBossUI()
{
	if (MainCharWidget)
	{
		MainCharWidget->EnableBossHPUI();
	}
}

void ARnCPlayerController::UpdateUI()
{
	if(MainCharWidget)
	{
		MainCharWidget->UpdateBindings();
		
		ARnCCharacter* MainChar = Cast<ARnCCharacter>(GetPawn());

		if(MainChar)
		{
			(MainChar->IsMeleeWeaponEquipped()) ? MainCharWidget->SetRangeWeaponWindowOpen(false) : MainCharWidget->SetRangeWeaponWindowOpen(true);
		}
		
	}
}
