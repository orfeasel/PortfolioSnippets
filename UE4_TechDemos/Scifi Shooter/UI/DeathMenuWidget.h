// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DeathMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API UDeathMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	/*Plays the entry animation of the death menu widget*/
	UFUNCTION(BlueprintImplementableEvent, Category = "DeathWidget")
	void Show();
	
};
