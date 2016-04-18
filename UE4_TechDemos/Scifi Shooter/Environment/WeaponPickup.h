// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Public/Weapons/Weapon.h"
#include "WeaponPickup.generated.h"

UCLASS()
class RNC_API AWeaponPickup : public AActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
		void OnActorOverlap(AActor* OverlapedActor);
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	/*The Box component responsible for checking any collisions*/
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

	/*The skeletal mesh of the weapon
	Updates every time we assign a new weapon using the Editor*/
	UPROPERTY(VisibleDefaultsOnly)
	USkeletalMeshComponent* WeaponSkeletalMesh;

	/*A basic rotating movement component*/
	URotatingMovementComponent* RotatingMovementComp;

	/*The Blueprint of the weapon that our character will unlock if he overlaps with this pickup*/
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponBP;
	
};
