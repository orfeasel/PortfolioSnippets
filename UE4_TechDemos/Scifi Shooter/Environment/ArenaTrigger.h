// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "Public/AI/BossCharacter.h"
#include "Public/AI/BossAIController.h"
#include "ArenaTrigger.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API AArenaTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
	
private:
	UFUNCTION()
	void OnOverlapedActor(AActor* OverlapedActor);

protected:

	/*Closes the arena doors*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Animations")
	void MoveArenaDoors();

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Boss")
	TSubclassOf<ABossAIController> BossConBP;

	UPROPERTY(EditAnywhere, Category="Boss")
	ABossCharacter* BossCharacterRef;
};
