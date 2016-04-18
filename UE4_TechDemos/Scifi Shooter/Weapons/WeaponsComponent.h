// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Public/Weapons/Weapon.h"
#include "Public/Weapons/RangeWeapon.h"
//#include "Public/Weapons/Knife.h"
//#include "Public/Weapons/AssaultRifle.h"
#include "WeaponsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RNC_API UWeaponsComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/*Weapons array. It's used to hold a reference to all available weapons*/
	TArray<AWeapon*> WeaponsArray;

	/*Returns a reference of the desired weapon*/
	AWeapon* GetWeaponBasedOnType(EWeaponType WeaponType);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TArray<TSubclassOf<AWeapon>> AvailableWeaponBlueprintReferences;

public:	
	// Sets default values for this component's properties
	UWeaponsComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	/*Returns a random weapon reference*/
	AWeapon* GetRandomWeaponReference(bool ShouldBeMelee);

	/*Makes the character to equip the desired weapon if possible*/
	UFUNCTION()
	void UpdateCharacterEquippedWeapon(EWeaponType WeaponToEquip);

	/*Increments the ammo of the desired weapon*/
	void UpdateWeaponAmmo(EWeaponType WeaponType, int32 AmmoToAdd);

	/*Unlocks the desired weapon*/
	void UnlockWeapon(EWeaponType WeaponType);

	void ResetComboMoveOnWeapon(EWeaponType WeaponType);

};
