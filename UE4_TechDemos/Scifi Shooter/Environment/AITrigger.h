// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "Public/AI/BaseEnemyCharacter.h"
#include "AITrigger.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API AAITrigger : public ATriggerBox
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnTriggerOverlap(AActor* OverlapedActor);

protected:
	/*The enemies that will be notified to attack the player when he enters this trigger.
	Mark the AI enemies from the level inside the editor.
	Once the player crosses the trigger the Enemy Character will switch from patrol mode to attack mode*/
	UPROPERTY(EditAnywhere)
	TArray<ABaseEnemyCharacter*> EnemyCharacterToNotify;
	
public:

	virtual void BeginPlay() override;
};
