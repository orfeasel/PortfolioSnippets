// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class RNC_API ABaseAIController : public AAIController
{
	GENERATED_BODY()


protected:

	/*Behavior component*/
	UBehaviorTreeComponent* BehaviorComp;

	/*Blackboard component*/
	UBlackboardComponent* BlackboardComp;

	/*Blackboard TargetKey option*/
	const FName TargetKey="CurrentTarget";

	/*Blackboard AIPatrolNode key option*/
	const FName PatrolNodeKey = "PatrolNode";


public:

	ABaseAIController();

	virtual void Possess(APawn* Pawn) override;

	virtual void UnPossess() override;

	/*Set the new target of the AI*/
	void SetNewTarget(AActor* ActorToTarget);

	/*Returns the current target of the AI*/
	AActor* GetCurrentTarget();

	/*Set the new patrol node of the AI*/
	void SetNewPatrolNode(AActor* Node);

	//------------------------------------------------------------------
	//AI related components
	//------------------------------------------------------------------

	/*Returns the blackboard component for this controller*/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() { return BlackboardComp; }

	/*Returns the behavior tree component for this controller*/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComp() { return BehaviorComp; }
	
};
