// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HealthPickup.generated.h"

UCLASS()
class RNC_API AHealthPickup : public AActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnActorOverlaped(AActor* OverlapedActor);

public:	
	// Sets default values for this actor's properties
	AHealthPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HealthKitSM;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleSystemComp;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* RotatingMovementComp;

	/*The health that will be added to the player if the pickup gets collected*/
	UPROPERTY(EditAnywhere)
	float HealthToProvide;
};
