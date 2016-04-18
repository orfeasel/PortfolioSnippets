// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/BaseEnemyCharacter.h"
#include "BossCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API ABossCharacter : public ABaseEnemyCharacter
{
	GENERATED_BODY()

private:
	const FName UpperHandSocket = "UpperHandSocket";

	const FName LowerHandSocket = "LowerHandSocket";

protected:
	/*This value is used in order to adjust the boss rotation a bit because it's animation will not
	face forward by default (bring in an animator ASAP!)*/
	UPROPERTY(EditAnywhere, Category = "Combat")
		float YawModifier = 10.f;
	
public:

	UFUNCTION(BlueprintCallable, Category="BossStats")
	float GetRemainingHealth() { return Health; }

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void Attack();

};
