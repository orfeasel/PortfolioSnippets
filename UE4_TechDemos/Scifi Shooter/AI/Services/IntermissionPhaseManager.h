// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "IntermissionPhaseManager.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API UIntermissionPhaseManager : public UBTService
{
	GENERATED_BODY()
	
public:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};

