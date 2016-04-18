// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "BossAIController.h"
#include "Public/AI/BossCharacter.h"
#include "Public/Environment/IntermissionNode.h"

void ABossAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	SetIntermissionPhaseStatus(false);

	int32 TotalIntermissions = HealthPercentForAnIntermissionPhase.Num();

	ActivatedIntermissionPhases.Init(false, TotalIntermissions);

	ABossCharacter* Boss = Cast<ABossCharacter>(InPawn);
	if (Boss)
	{
		BossMaxHealth = Boss->GetRemainingHealth();
		SetNewTarget(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AIntermissionNode::StaticClass(), OutActors);
		
		for (auto It = OutActors.CreateIterator(); It; It++)
		{
			if((*It) && (*It)->IsA<AIntermissionNode>())
			{
				IntermissionNodesArray.Add(Cast<AIntermissionNode>(*It));
			}
		}

	}
}


//-------------------------------------------------------------------------------------
//Determines if the boss should go into an intermission phase.
//If so, makes sure to find an available node and update the associated blackboard
//-------------------------------------------------------------------------------------
void ABossAIController::PerformIntermissionCheck()
{
	ABossCharacter* Boss = Cast<ABossCharacter>(GetPawn());

	if(Boss)
	{
		
		//If the boss is already in an intermission phase, determine if it needs to move into combat
		if(BlackboardComp && BlackboardComp->GetValueAsBool(IntermissionPhaseKey))
		{
			float CurrentDurationOfIntermission = IntermissionPhaseStartTime - GetWorld()->GetRealTimeSeconds();

			CurrentDurationOfIntermission = FMath::Abs<float>(CurrentDurationOfIntermission);

			//The boss depleted the intermission timer, disable the intermission and move into combat
			if (CurrentDurationOfIntermission >= IntermissionDuration)
			{
				SetIntermissionPhaseStatus(false);
				bIsInIntermission = false;

				//GLog->Log("going into fight phase with health:" + FString::SanitizeFloat(Boss->GetRemainingHealth()));

				return;
			}
		}
		

		//Determine if the boss should go into an intermission phase
		int32 NextIntermissionPhase = ActivatedIntermissionPhases.Find(false);
		if(NextIntermissionPhase!=INDEX_NONE)
		{
			//The next health percent that corresponds to the next intermission phase
			float HealthPercent = HealthPercentForAnIntermissionPhase[NextIntermissionPhase];

			//GLog->Log("Health percent for next intermission:" + FString::SanitizeFloat(HealthPercent));

			
			if (((Boss->GetRemainingHealth() / BossMaxHealth) * 100) <= HealthPercent)
			{
				//Activating the intermission phase
				ActivatedIntermissionPhases[NextIntermissionPhase] = true;

				SetIntermissionPhaseStatus(true);
				bIsInIntermission = true;

				//GLog->Log("going into intermission phase with health:" + FString::SanitizeFloat(Boss->GetRemainingHealth()));
			}
		}
	}
}

//-------------------------------------------------------------------------------------
//Returns the current intermission node
//-------------------------------------------------------------------------------------
AIntermissionNode* ABossAIController::GetCurrentIntermissionNode()
{
	if(BlackboardComp)
	{
		AIntermissionNode* CurrentNode = Cast<AIntermissionNode>(BlackboardComp->GetValueAsObject(IntermissionNodeKey));
		return CurrentNode;
	}
	return nullptr;
}

//-------------------------------------------------------------------------------------
//Updates the intermission phase status on blackboard
//-------------------------------------------------------------------------------------
void ABossAIController::SetIntermissionPhaseStatus(bool Status)
{
	if(BlackboardComp && Status)
	{
		BlackboardComp->SetValueAsBool(IntermissionPhaseKey, Status);

		FindTheFarthestrIntermissionNode();

		IntermissionPhaseStartTime = GetWorld()->GetRealTimeSeconds();
	}
	else if (BlackboardComp && !Status)
	{
		//Updating the values of our blackboard to match the current state of the fight
		BlackboardComp->SetValueAsBool(IntermissionPhaseKey, Status);

		BlackboardComp->SetValueAsObject(IntermissionNodeKey, nullptr);
	}
}

//-------------------------------------------------------------------------------------
//Finds the most distant intermission node
//-------------------------------------------------------------------------------------
void ABossAIController::FindTheFarthestrIntermissionNode()
{
	//A basic search algorithm for the max distance
	if(IntermissionNodesArray.IsValidIndex(0) && GetPawn())
	{
		//Get the first available node and determine its distance from our pawn
		float MaxDistance = (IntermissionNodesArray[0]->GetActorLocation() - GetPawn()->GetActorLocation()).Size();
		AIntermissionNode* FarthestNode = IntermissionNodesArray[0];

		//Compare every other node to determine the true max distance
		for (int32 Index = 1; Index < IntermissionNodesArray.Num();Index++)
		{
			if(IntermissionNodesArray[Index] && MaxDistance<(IntermissionNodesArray[Index]->GetActorLocation() - GetPawn()->GetActorLocation()).Size())
			{
				MaxDistance = (IntermissionNodesArray[Index]->GetActorLocation() - GetPawn()->GetActorLocation()).Size();

				FarthestNode = IntermissionNodesArray[Index];
			}
		}

		//Update the values of our blackboard with our intermission node

		if (BlackboardComp) BlackboardComp->SetValueAsObject(IntermissionNodeKey, FarthestNode);

	}
}
