// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "RoguelikeChar.h"
#include "ZombieController.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKESURVIVAL_API AZombieController : public AAIController
{
	GENERATED_BODY()

	/*Behavior Tree component reference*/
	UBehaviorTreeComponent* BehaviorComp;
	
	/*************Blackboard************/
	
	/*Blackboard comp ref*/
	UBlackboardComponent* BlackboardComp;

	/*Blackboard key*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName PlayerBlackboardKey="Player";

	/*Custom constructor*/
	AZombieController(const class FObjectInitializer& ObjectInitializer);

public:
	/*Initialization of blackboard values and behavior tree start point*/
	virtual void Possess(APawn* Pawn) override;

	/*Stops the logic of the AI*/
	virtual void UnPossess() override;

	/*Gets a reference of our character*/
	ARoguelikeChar* GetTarget();
};
