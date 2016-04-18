// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Public/Projectiles/WeaponProjectile.h"
#include "Public/AnimInstances/BaseEnemyAnimInstance.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS(ABSTRACT)
class RNC_API ABaseEnemyCharacter : public ACharacter
{
	GENERATED_BODY()
		
private:
	/*The last time the last attack occured*/
	float LastAttack;

protected:

	UBaseEnemyAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BaseDamageAmount;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float Health;

	/*This value is used in order to avoid spamming attacks when the player is firing. A value of 1 means that the 
	enemy can be damaged one time per second*/
	UPROPERTY(EditAnywhere, Category = "Combat")
	float SpamAttack = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	int32 XpReward;

public:
	// Sets default values for this character's properties
	ABaseEnemyCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//--------------------------------------------------------------
	//Combat related
	//--------------------------------------------------------------

	virtual void Attack() { unimplemented(); }

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	inline bool IsAlive() { return Health > 0; }


	FORCEINLINE int32 GetXpReward() { return XpReward; }

	/*Behavior Tree for the current character*/
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;
	
};
