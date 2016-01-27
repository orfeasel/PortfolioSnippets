// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickupable.generated.h"

UCLASS(ABSTRACT)
class ROGUELIKESURVIVAL_API APickupable : public AActor
{
	GENERATED_BODY()

protected:

	/*Called when an Actor overlaps with a pickupable object*/
	UFUNCTION()
	virtual void ActorOverlaped(AActor* Other);

	/*Destroys the pickupable after calculated seconds*/
	UFUNCTION()
	void Kill();

	/*The seconds that elapse in order to destroy the actor when it's overlapped*/
	UPROPERTY(EditAnywhere, Category = "Destroy")
	float SecondsToDestroy;

	
public:	
	// Sets default values for this actor's properties
	APickupable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/*The static mesh of the pickupable*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	/*The rotating movement component we use to simulate animations*/
	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* RotatingMovementComponent;

	/*The primary particle emmiter*/
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ParticleEmitter;

	/*The audio comp that plays when the pickupable gets collected*/
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	
	
};
