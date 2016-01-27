// Fill out your copyright notice in the Description page of Project Settings.

#include "CavemanTestField.h"
#include "WolfCharacter.h"
#include "WolfAIController.h"
#include "BTTask_WolfAttack.h"



EBTNodeResult::Type UBTTask_WolfAttack::ExecuteTask(UBehaviorTreeComponent& OwnedTree, uint8* Node)
{
	AWolfAIController* Controller = Cast<AWolfAIController>(OwnedTree.GetOwner());
	if (!Controller)
	{
		return EBTNodeResult::Failed;
	}

	Controller->StopMovement();

	AWolfCharacter* Wolf = Cast<AWolfCharacter>(Controller->GetPawn());
	if (Wolf)
	{
		//Animation handling
		if (Wolf->CanPerformAttack())
		{
			//GLog->Log("Performing Attack");
			Wolf->MyAnimInstance->CurrentState = EWolfStates::Attack;
			//Wolf->PerformMeleeHit();
		}
		else
		{
			//GLog->Log("Not performing Attack");
			Wolf->MyAnimInstance->CurrentState = EWolfStates::CombatStance;
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
