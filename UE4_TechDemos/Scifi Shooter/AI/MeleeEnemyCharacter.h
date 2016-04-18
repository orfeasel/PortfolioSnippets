// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/BaseEnemyCharacter.h"
#include "MeleeEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API AMeleeEnemyCharacter : public ABaseEnemyCharacter
{
	GENERATED_BODY()
	
private:

	const FName UpperSphereSocket = "UpperSphereSocket";

	const FName LowerSphereSocket = "LowerSphereSocket";

	const FName ForceAttackKey = "ForceAttack";
	
public:

	AMeleeEnemyCharacter();

	virtual void Tick(float DeltaTime) override;

	/*Attacks the player*/
	virtual void Attack() override;
	
	/*Cancel the patrol system and hunt down the player*/
	void EnableHunting();

protected:
	UPROPERTY(EditAnywhere, Category=Combat)
	float AttackRaycastRadius;
};
