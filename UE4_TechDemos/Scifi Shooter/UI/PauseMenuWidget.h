// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	/*Plays the intro animation and adds the Widget to viewport*/
	UFUNCTION(BlueprintImplementableEvent, Category = "PauseMenuWidget")
	void Show();

	/*Plays the outro animation and removes the Widget from the viewport*/
	UFUNCTION(BlueprintImplementableEvent, Category = "PauseMenuWidget")
	void Hide();
	
};
