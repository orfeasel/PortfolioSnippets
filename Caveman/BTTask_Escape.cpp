// Fill out your copyright notice in the Description page of Project Settings.

#include "CavemanTestField.h"
#include "BTTask_Escape.h"
#include "MyCharacter_FirstTry.h"
#include "WolfAIController.h"
#include "WolfCharacter.h"
#include "PathNode.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"



EBTNodeResult::Type UBTTask_Escape::ExecuteTask(UBehaviorTreeComponent& OwnedTree, uint8* Node)
{
	AWolfAIController* Controller = Cast<AWolfAIController>(OwnedTree.GetAIOwner());
	if (!Controller)
	{
		GLog->Log("Controller fail");
		return EBTNodeResult::Failed;
	}


	FName SensedTargetKey = "SensedTarget";
	AMyCharacter_FirstTry* Caveman = Cast<AMyCharacter_FirstTry>(OwnedTree.GetBlackboardComponent()->GetValueAsObject(SensedTargetKey));
	if (Caveman)
	{
		TArray<FHitResult> HitResults;
		//Start and End Location of the capsule
		FVector StartLocation = Caveman->GetActorLocation();
		FVector EndLocation = StartLocation;
		EndLocation.Z += Caveman->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2;

		//Collision Channel
		ECollisionChannel ECC = ECollisionChannel::ECC_WorldDynamic;

		//Collision Shape
		FCollisionShape CollisionShape;

		//Making Collision in sphere
		CollisionShape.ShapeType = ECollisionShape::Sphere;
		CollisionShape.SetSphere(350);

		//Sweeping for possible escape nodes
		if (Caveman->GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat::FQuat(), ECC, CollisionShape))
		{
			//DrawDebugSphere(Caveman->GetWorld(), ((EndLocation - StartLocation) / 2) + StartLocation, CollisionShape.GetSphereRadius(), 100, FColor::Green, true);
			TArray<AActor*> AllPathNodes;
			UGameplayStatics::GetAllActorsOfClass(Caveman->GetWorld(), APathNode::StaticClass(), AllPathNodes);

			APathNode* NextNode;
			NextNode = Cast<APathNode>(AllPathNodes[FMath::RandRange(0, AllPathNodes.Num() - 1)]);
			//Finding next possible node
			while (NextNode->bIsCurrentlyOccupied && AllPathNodes.Contains(NextNode))
			{
				NextNode = Cast<APathNode>(AllPathNodes[FMath::RandRange(0, AllPathNodes.Num() - 1)]);
				GLog->Log("Fear success");
			}

			//Occupy the node
			NextNode->bIsCurrentlyOccupied = true;
			
			//Set the new node in the blackboard
			OwnedTree.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), NextNode);
			//OwnedTree.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(Bl)
			
			GLog->Log("Success after while");
			return EBTNodeResult::Succeeded;

		}

	}

	GLog->Log("General Fail");
	return EBTNodeResult::Failed;
}
