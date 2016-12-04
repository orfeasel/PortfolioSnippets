// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/Actor.h"
#include "Components/TimelineHandleComponent.h"
#include "Character/ButtlerCharacter.h"
#include "IntroBoat.generated.h"

UCLASS()
class PROJECTNA_API AIntroBoat : public AActor
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void MoveToShore(FVector NewLocation);

	UFUNCTION()
	void OnShoreArrived();

protected:

	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BoatSM;

	UPROPERTY(VisibleAnywhere)
	UTimelineHandleComponent* TimelineHandleComp;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* LeftBoxComp;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* RightBoxComp;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* FrontBoxComp;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* RearBoxComp;

	UPROPERTY(EditAnywhere)
	AButtlerCharacter* ButtlerCharacterRef;
	
public:	

	// Sets default values for this actor's properties
	AIntroBoat();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
