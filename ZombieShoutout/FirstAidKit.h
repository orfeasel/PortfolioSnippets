// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickupable.h"
#include "RoguelikeChar.h"
#include "FirstAidKit.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKESURVIVAL_API AFirstAidKit : public APickupable
{
	GENERATED_BODY()
	
private:
	/*Health amount left in order to apply later. Used only in overtime heal*/
	float HealthAmountLeftToApply;

	/*Reference to our character*/
	ARoguelikeChar* OurCharacter;

	/*Applies heal overtime*/
	UFUNCTION()
	void ApplyOvertimeHeal();

protected:
	UFUNCTION()
	virtual void ActorOverlaped(AActor* Other) override;

public:
	AFirstAidKit();

	virtual void BeginPlay() override;

	/*The total health that the player will receive. If overtime heal is activated
	the player will receive the specified amount overtime*/
	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthBonus;

	/*If you want to activate the overtime heal, enable this and set the overtime
	heal duration.*/
	UPROPERTY(EditAnywhere, Category = "Health")
	bool bIsOvertimeHeal;

	/*The duration of the overtime heal*/
	UPROPERTY(EditAnywhere, Category = "Health")
	float OvertimeHealDuration;

	/*The real time seconds that the overtime heal ticks*/
	UPROPERTY(EditAnywhere, Category = "Health")
	float TickOfOvertimeHeal;

	/*The material of the health kit when it's overtime heal*/
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	UMaterialInterface* OvertimeHealthMaterial;

	void UpdateMaterial();
	
};
