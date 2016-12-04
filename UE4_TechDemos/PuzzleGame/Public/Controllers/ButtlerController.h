// Orfeas Eleftheriou 2016

#pragma once

#include "AIController.h"
#include "ButtlerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API AButtlerController : public AAIController
{
	GENERATED_BODY()

private:
	
	UBehaviorTreeComponent* BehaviorTreeComp;

	UBlackboardComponent* BlackboardComp;
	
protected:

	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* ExecutingBehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationToGoKey = "LocationToGoKey";

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName ActorToFaceKey = "ActorToFace";

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName ExitActorKey = "ExitActor";

	virtual void Possess(APawn* InPawn) override;

public:

	AButtlerController();

	void SetMoveLocation(AActor* ActorToMoveAt);

	void SetActorToFace(AActor* ActorToFace);

	void SetExitActor(AActor* ExitActor);
};
