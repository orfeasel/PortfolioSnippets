// Fill out your copyright notice in the Description page of Project Settings.

#include "RoguelikeSurvival.h"
#include "ZombieController.h"
#include "ZombieCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"


AZombieController::AZombieController(const class FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//Initializing behavior tree component and blackboard component
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, "BehaviorComp");

	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, "BlackboardComp");
}

void AZombieController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	AZombieCharacter* Zombie = Cast<AZombieCharacter>(Pawn);

	if (Zombie)
	{
		//Initialize blackboard
		if (Zombie->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(Zombie->BehaviorTree->BlackboardAsset));
		}

		//Get player and hold a reference of it
		TArray<AActor*> ActorsArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoguelikeChar::StaticClass(), ActorsArray);

		if (ActorsArray.IsValidIndex(0))
		{
			ARoguelikeChar* Player = Cast<ARoguelikeChar>(ActorsArray[0]);
			if (Player)
			{
				BlackboardComp->SetValueAsObject(PlayerBlackboardKey, Player);
			}
			
		}
		//Start the tree
		BehaviorComp->StartTree(*Zombie->BehaviorTree);
	}
}

void AZombieController::UnPossess()
{
	Super::UnPossess();

	this->BehaviorComp->StopTree();
}

ARoguelikeChar* AZombieController::GetTarget()
{
	ARoguelikeChar* Player=Cast<ARoguelikeChar>(BlackboardComp->GetValueAsObject(PlayerBlackboardKey));
	return Player;
}