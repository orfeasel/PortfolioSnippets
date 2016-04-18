// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WeaponProjectile.generated.h"

UCLASS(ABSTRACT)
class RNC_API AWeaponProjectile : public AActor
{
	GENERATED_BODY()


private:
	UFUNCTION()
	void OnBoxOverlap(AActor* OverlapedActor,UPrimitiveComponent* OverlapedComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	/*The damage amount that this projectile applies to hit enemies*/
	float DamageAmount;

	/*Called when the projectile stops*/
	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);
	
public:	
	// Sets default values for this actor's properties
	AWeaponProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/*This component is just a  wrapper for collision generation.*/
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

	/*The particle system for the current projectile*/
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BulletComp;
	
	/*The component responsible for the movement of the projectile*/
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMoveComp;

	
};
