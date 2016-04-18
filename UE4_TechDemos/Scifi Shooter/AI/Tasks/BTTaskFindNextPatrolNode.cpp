// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "BTTaskFindNextPatrolNode.h"
#include "Public/Environment/AIPatrolNode.h"
#include "Public/AI/MeleeEnemyCharacter.h"
#include "Public/AI/BaseAIController.h"

#define MAX_ATTEMPTS 500

//-------------------------------------------------------------------------------------
//Finds a random patrol node if possible
//-------------------------------------------------------------------------------------
EBTNodeResult::Type UBTTaskFindNextPatrolNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* AICon = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if(AICon)
	{
		TArray<AActor*> AIPatrolNodes;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolNode::StaticClass(), AIPatrolNodes);

		bool bFoundNextNode = false;
		AAIPatrolNode* NextNodeToOccupy = nullptr;
		int32 CurrentAttempts = 0;

		//Find the next available node
		//In case someone places more AI enemies than patrol nodes you'll a runtime error will occur.
		//That why i'm using a max attempts number in order to avoid that. Worst case scenario some AIs
		//won't move, however the game will continue to run!
		while(!bFoundNextNode && CurrentAttempts<MAX_ATTEMPTS)
		{
			CurrentAttempts++;

			NextNodeToOccupy = Cast<AAIPatrolNode>(AIPatrolNodes[FMath::RandRange(0, AIPatrolNodes.Num() - 1)]);
			if(NextNodeToOccupy && !NextNodeToOccupy->IsOccupied())
			{
				AICon->SetNewPatrolNode(NextNodeToOccupy);
				//bFoundNextNode = true;
				return EBTNodeResult::Succeeded;
			}
		}

	}

	return EBTNodeResult::Failed;

}


