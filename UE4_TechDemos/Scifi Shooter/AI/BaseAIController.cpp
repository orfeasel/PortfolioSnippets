// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "Public/AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Public/AI/BaseEnemyCharacter.h"
#include "RnCCharacter.h"
#include "Public/Environment/AIPatrolNode.h"

ABaseAIController::ABaseAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorComp"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));


}

void ABaseAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	ABaseEnemyCharacter* EnemyChar = Cast<ABaseEnemyCharacter>(Pawn);

	if (EnemyChar && EnemyChar->BehaviorTree->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*EnemyChar->BehaviorTree->BlackboardAsset);
		GLog->Log("Initialized blackboard.");

		BehaviorComp->StartTree(*EnemyChar->BehaviorTree);

	}

}

void ABaseAIController::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

//-------------------------------------------------------------------------------------
//Sets a new target
//-------------------------------------------------------------------------------------
void ABaseAIController::SetNewTarget(AActor* ActorToTarget)
{
	BlackboardComp->SetValueAsObject(TargetKey, ActorToTarget);
}

//-------------------------------------------------------------------------------------
//Returns the current target of the AI
//-------------------------------------------------------------------------------------
AActor* ABaseAIController::GetCurrentTarget()
{
	AActor* CurrentTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey));
	return CurrentTarget;
}

//-------------------------------------------------------------------------------------
//Sets a new patrol node for the AI
//-------------------------------------------------------------------------------------
void ABaseAIController::SetNewPatrolNode(AActor* Node)
{
	//Find the old patrol node and un-occupy it
	AAIPatrolNode* OldPatrolNode = Cast<AAIPatrolNode>(BlackboardComp->GetValueAsObject(PatrolNodeKey));

	if (OldPatrolNode) OldPatrolNode->SetOccupyStatus(false);

	AAIPatrolNode* NewPatrolNode = Cast<AAIPatrolNode>(Node);

	//Occupy the new node and assign its value to the Blackboard
	if(NewPatrolNode)
	{
		NewPatrolNode->SetOccupyStatus(true);
		BlackboardComp->SetValueAsObject(PatrolNodeKey, Node);
	}

	
}
