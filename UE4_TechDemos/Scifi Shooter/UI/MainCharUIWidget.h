// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MainCharUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API UMainCharUIWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	/*The actual health value converted in percentage which is provided for the UI Binding*/
	UPROPERTY(BlueprintReadOnly)
	float HealthValue;

	UPROPERTY(BlueprintReadOnly)
	int32 AmmoLeft;

	
public:

	/*Updates the health value of the character. Use it for UI Binding*/
	void UpdateHealthValue();

	/*Updates the UI bindings*/
	void UpdateBindings();

	/*Shows/hides the ammo left pane*/
	UFUNCTION(BlueprintImplementableEvent, Category="MainCharUI")
	void SetRangeWeaponWindowOpen(bool Status);

	/*Shows the boss hud*/
	UFUNCTION(BlueprintImplementableEvent, Category = "MainCharUI")
	void EnableBossHPUI();
};
