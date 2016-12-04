// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "ButtlerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"



void AButtlerController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	BlackboardComp->InitializeBlackboard(*ExecutingBehaviorTree->BlackboardAsset);

	BehaviorTreeComp->StartTree(*ExecutingBehaviorTree);
}

AButtlerController::AButtlerController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorTreeComp"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));
}

void AButtlerController::SetMoveLocation(AActor* ActorToMoveAt)
{
	if (!ActorToMoveAt) return;

	BlackboardComp->SetValueAsObject(LocationToGoKey, ActorToMoveAt);
}

void AButtlerController::SetActorToFace(AActor* ActorToFace)
{
	if (!ActorToFace) return;

	BlackboardComp->SetValueAsObject(ActorToFaceKey, ActorToFace);
}

void AButtlerController::SetExitActor(AActor* ExitActor)
{
	if (!ExitActor) return;

	BlackboardComp->SetValueAsObject(ExitActorKey, ExitActor);
}

