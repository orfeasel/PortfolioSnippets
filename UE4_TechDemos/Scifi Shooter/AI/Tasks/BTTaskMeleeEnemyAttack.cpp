// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "Public/AI/Tasks/BTTaskMeleeEnemyAttack.h"
#include "Public/AI/BaseAIController.h"
#include "Public/AI/MeleeEnemyCharacter.h"
#include "Public/AI/BaseEnemyCharacter.h"
#include "RnCCharacter.h"

//-------------------------------------------------------------------------------------
//Performs an Attack function based on the associated Behavior Tree logic
//-------------------------------------------------------------------------------------
EBTNodeResult::Type UBTTaskMeleeEnemyAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* AICon = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if(AICon)
	{
		ABaseEnemyCharacter* Char = Cast<ABaseEnemyCharacter>(AICon->GetPawn());
		ARnCCharacter* MainChar = Cast<ARnCCharacter>(AICon->GetCurrentTarget());

		if(Char && MainChar && MainChar->IsAlive())
		{
			Char->Attack();
		}
		else
		{
			AICon->UnPossess();
			return EBTNodeResult::Aborted;
		}
	}


	return EBTNodeResult::Succeeded;
}

