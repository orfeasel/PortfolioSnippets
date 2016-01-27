// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Trap.generated.h"

UCLASS()
class CAVEMANTESTFIELD_API ATrap : public AActor
{
	GENERATED_BODY()

private:
	//If true the trap does dmg to wolfs
	bool bIsActivated = false;
	
public:	
	// Sets default values for this actor's properties
	ATrap();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/*When the enemy overlaps this trigger it takes dmg*/
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TrapCollider;

	/*Static mesh of actual trap*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TrapStaticMesh;

	/*Activate or Deactivate trap*/
	UFUNCTION(BlueprintCallable, Category = "Trap Mechanic")
	void SetTrapEnable(bool Status);

	/*Trap will be activated after x seconds after placement*/
	UPROPERTY(EditDefaultsOnly, Category = "Trap Mechanic")
	float TrapActivationTimer=10;

	/*Called when someone is overlaping with the trap*/
	UFUNCTION()
	void TriggerTrap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
