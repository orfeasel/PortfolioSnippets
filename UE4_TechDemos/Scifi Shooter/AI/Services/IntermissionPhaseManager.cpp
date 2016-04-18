// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "IntermissionPhaseManager.h"
#include "Public/AI/BossCharacter.h"
#include "Public/AI/BossAIController.h"


//-------------------------------------------------------------------------------------
//Performs an intermission check based on the frequency entered in the Behavior Tree
//-------------------------------------------------------------------------------------
void UIntermissionPhaseManager::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ABossAIController* Con = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	if(Con)
	{
		Con->PerformIntermissionCheck();
	}


}

