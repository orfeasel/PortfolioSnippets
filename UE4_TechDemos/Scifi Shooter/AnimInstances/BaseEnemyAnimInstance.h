// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class RNC_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	/*Keeps a reference of the Pawn's Speed to change animation based on 1D Blend Space*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyAnimationProperties")
	float CharacterSpeed;

	/*Updates the speed of the character*/
	UFUNCTION(BlueprintCallable, Category = "EnemyAnimationProperties")
	void UpdatePawnSpeed();

	/*Attack montage reference*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMontages")
	UAnimMontage* AttackMontage;

	/*Get hit montage reference*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyMontages")
	UAnimMontage* GetHitMontage;

	/*True if the enemy should raycast for attacks*/
	bool bDamageRaycast;

	/*True if the enemy is dead*/
	UPROPERTY(BlueprintReadOnly, Category = "EnemyAnimationProperties")
	bool bIsDead;
	
public:
	virtual void PlayAttackAnimation();

	/*Handles the get hit animation of the enemy*/
	UFUNCTION(BlueprintCallable, Category = "Montages")
	virtual void PlayGetHitAnimation();

	/*True if the melee enemy is raycasting for attack*/
	inline bool GetDamageRaycastStatus() { return bDamageRaycast; }

	/*True if you want to raycast for collisions*/
	UFUNCTION(BlueprintCallable, Category = "EnemyAnimNotifies")
	void SetDamageRaycastStatus(bool Status) { bDamageRaycast = Status; }

	/*Sets the animation state to dead*/
	inline void Die() { bIsDead = true; }
	
};
