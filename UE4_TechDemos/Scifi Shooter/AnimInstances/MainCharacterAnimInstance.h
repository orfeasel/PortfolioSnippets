// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

#define BATTLECRY_MEDIAN_DURATION 2.5

/**
 * 
 */
UCLASS()
class RNC_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	/*Keeps a reference of the Character's Speed to change animation based on 1D Blend Space*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacterAnimationProperties")
	float CharacterSpeed;

	/*Keeps a reference of the Character's falling status*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacterAnimationProperties")
	bool bIsFalling;

	/*Returns the Character's movement speed*/
	UFUNCTION(BlueprintCallable, Category = "MainCharacterAnimationProperties")
	float GetMainCharacterSpeed();
	
	/*Updates tha falling status of the Character*/
	UFUNCTION(BlueprintCallable, Category = "MainCharacterAnimationProperties")
	void UpdateFallingStatus();

	/***************Melee Combo System***************/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MeleeCombatNotifiers")
	bool bAcceptsSecondAttackInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MeleeCombatNotifiers")
	bool bAcceptsThirdAttackInput;

	/***************Melee Combo System***************/

	//------------------------------------------------------------------------------
	//Montages
	//------------------------------------------------------------------------------

	/*The Animation Montage which contains all the melee attack moves*/
	UPROPERTY(EditAnywhere,Category="MainCharAnimMontages")
	UAnimMontage* MeleeAttackAnimMontage;

	/*The Animation Montage which plays when the character takes dmg while in melee state*/
	UPROPERTY(EditAnywhere, Category = "MainCharAnimMontages")
	UAnimMontage* MeleeGetHitAnimMontage;

	/*The Animation Montage which plays when the character takes dmg while in range state*/
	UPROPERTY(EditAnywhere, Category = "MainCharAnimMontages")
	UAnimMontage* RangeGetHitAnimMontage;

	UPROPERTY(EditAnywhere, Category = "MainCharAnimMontages")
	UAnimMontage* IdleShootingAnimMontage;

	/*Returns true if the player fires while in idle mode*/
	UFUNCTION(BlueprintCallable, Category = "MainCharacterAnimationUpdates")
	void IsFiringWhileIdle();

	UFUNCTION(BlueprintCallable, Category = "MainCharacterAnimationUpdates")
	void UpdateMeleeStatus();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainCharacterAnimationProperties")
	bool bIsMelee;

	/*The movement speed that tha player is considered as idle while holding a gun*/
	UPROPERTY(EditAnywhere, Category = "MainCharacterAnimationProperties")
	float GunIdleThreshold;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainCharacterAnimationProperties")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainChararacterAnimationProperties")
	bool bIsInBattlecry;

public:

	bool IsMeleeMontagePlaying() { return Montage_IsPlaying(MeleeAttackAnimMontage); }

	/*Enables the Attack Montage*/
	void Attack();

	/*Enables the Get Hit Animation if it's not playing*/
	void PlayGetHitAnimation();

	/*Updates Attack Montage if the player does a combo move*/
	void ContinueMeleeCombo();

	void EnableBattlecry() { bIsInBattlecry = true; }

	void PlayDeathAnimation();

};
