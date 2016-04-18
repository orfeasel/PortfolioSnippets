// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "Public/Weapons/Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponSM = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponSM"));

	SetRootComponent(WeaponSM);

	MaxAmmo = 9999;

	bIsUnlocked = false;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentWeaponStats.SetDataTable(WeaponStatsDataTable);

}


//-------------------------------------------------------------------------------------
//Fetches the total damage amount of the weapon based on its level
//-------------------------------------------------------------------------------------
float AWeapon::GetTotalDamageAmount()
{
	if(WeaponStatsDataTable)
	{
		return CurrentWeaponStats.GetDamageOnCurrentLevel();
	}
	return 0;
}

//-------------------------------------------------------------------------------------
//Increases the current ammo of the weapon
//-------------------------------------------------------------------------------------
void AWeapon::IncreaseCurrentAmmo(int32 AmmoToAdd)
{
	(CurrentAmmo + AmmoToAdd < MaxAmmo) ? CurrentAmmo += AmmoToAdd : CurrentAmmo = MaxAmmo;
}

//-------------------------------------------------------------------------------------
//Unlocks the weapon so it can be equipped by the player
//-------------------------------------------------------------------------------------
void AWeapon::Unlock()
{
	bIsUnlocked = true;
}

//-------------------------------------------------------------------------------------
//Increases the XP of the weapon by a given value
//-------------------------------------------------------------------------------------
void AWeapon::AddWeaponXP(int32 XpToAdd)
{
	CurrentWeaponStats.AddXP(XpToAdd);
}

//-------------------------------------------------------------------------------------
//Increases the combo move of the weapon - only for melee
//-------------------------------------------------------------------------------------
void AWeapon::IncreaseComboMove()
{
	(CurrentComboMove < MELEE_MAX_COMBO_MOVES) ? CurrentComboMove++ : CurrentComboMove = INITIAL_COMBO_MOVE_COUNT;
}
