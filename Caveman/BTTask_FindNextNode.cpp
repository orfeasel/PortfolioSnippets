// Fill out your copyright notice in the Description page of Project Settings.

#include "CavemanTestField.h"
#include "BTTask_FindNextNode.h"
#include "WolfAIController.h"
#include "WolfCharacter.h"
#include "PathNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
/* This contains includes all key types like UBlackboardKeyType_Vector used below. */
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"



EBTNodeResult::Type UBTTask_FindNextNode::ExecuteTask(UBehaviorTreeComponent& OwnerTree, uint8* Node)
{
	AWolfAIController* Controller = Cast<AWolfAIController>(OwnerTree.GetAIOwner());
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	FName Key = "NodeToMoveTo";

	APathNode* CurrentNode = Cast<APathNode>(OwnerTree.GetBlackboardComponent()->GetValueAsObject(Key));

	TArray<AActor*> WolfNodes;

	APathNode* NextNode = nullptr;

	if (CurrentNode)
	{
		//Turn of the occupie status because we will switch no in a moment
		CurrentNode->bIsCurrentlyOccupied = false;
	}

	//Get All actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APathNode::StaticClass(), WolfNodes);

	//Get a new random
	NextNode = Cast<APathNode>(WolfNodes[FMath::RandRange(0, WolfNodes.Num() - 1)]);
	while (NextNode->bIsCurrentlyOccupied)
	{
		NextNode = Cast<APathNode>(WolfNodes[FMath::RandRange(0, WolfNodes.Num() - 1)]);
	}

	//Occupy the node
	NextNode->bIsCurrentlyOccupied = true;
	//Set the new node in the blackboard
	OwnerTree.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), NextNode);
	//Success
	return EBTNodeResult::Succeeded;

	if (WolfNodes.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
