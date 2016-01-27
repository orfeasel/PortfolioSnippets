// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_WolfAttack.generated.h"

/**
 * 
 */
UCLASS()
class CAVEMANTESTFIELD_API UBTTask_WolfAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnedTree, uint8* Node) override;
	
	
};
