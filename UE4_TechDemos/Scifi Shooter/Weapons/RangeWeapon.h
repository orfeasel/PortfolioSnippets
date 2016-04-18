// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapons/Weapon.h"
#include "Projectiles/WeaponProjectile.h"
#include "RangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API ARangeWeapon : public AWeapon
{
	GENERATED_BODY()

private:
	float LastTimeWeaponWasFired;

	/*The name of the muzzle socket for the current weapon
	/*By convetion, it's the same for all range weapons*/
	FName MuzzleSocket="Muzzle";
	
public:
	ARangeWeapon();

	/*Fires the weapon (if fire rate allows it) and applies damage to any hit enemies*/
	void Fire();

	FORCEINLINE UStaticMesh* GetAmmoPickupSM() { return AmmoPickupSM; }

	FORCEINLINE UParticleSystem* GetParticleSystemBasedOnLevel() { return CurrentWeaponStats.GetParticleSystem(); }

	FORCEINLINE FName GetMuzzleSocketName() { return MuzzleSocket; }

protected:

	/*The max distance that the bullets of the weapon will travel*/
	UPROPERTY(EditAnywhere, Category = "Stats")
	float BulletMaxTravelDistance;
	
	/*The weapon fire rate in seconds, ie 1 means that the weapon
	can fire every second.*/
	UPROPERTY(EditAnywhere, Category = "Stats")
	float FireRate;

	/*The audio component responsible for the fire sound*/
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComp;

	/*The projectile that will be spanwed at the weapon's muzzle*/
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<AWeaponProjectile> WeaponProjectile;

	/*Projectile's speed multiplier*/
	UPROPERTY(EditAnywhere,Category="Weapon")
	float ProjectileSpeedMultiplier;

	/*The projectiles should get spawned a bit infront of the muzzle so they won't get stuck*/
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ProjectileSpawnLocationMultiplier;

	/*The static mesh of the ammo pickup*/
	UPROPERTY(EditAnywhere, Category = Weapon)
	UStaticMesh* AmmoPickupSM;
	
};
