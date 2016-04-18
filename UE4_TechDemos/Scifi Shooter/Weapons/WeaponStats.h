// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "WeaponStats.generated.h"


USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

private:
	/*The current level of the weapon*/
	int32 CurrentLevel;

	/*Current xp*/
	int32 CurrentXP;

	/*Data table associated with the specific instance of the weapon*/
	UDataTable* DataTable;

	/*Updates the stats of the weapon based on the given row number of the Data Table which corresponds to this instance*/
	void UpdateStats(int32 DataTableRow);

	/*Particle system used for ranged weapons only*/
	UParticleSystem* ParticleSystem;

protected:
	/*The XP Needed to upgrade the weapon's level*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 XpToNextLevel;

	/*The Damage that we weapon performs on the current level*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageOnCurrentLevel;

	/*The Particle System (if any) associated with the specific weapon's level - leave this empty for melee weapons*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<UParticleSystem> ParticleSystemOnCurrentLevel;

public:
	
	FWeaponStats();

	/*Associates the data table with the current instance*/
	void SetDataTable(UDataTable* WeaponDataTable);

	/*Increases the weapon's xp. If necessary upgrades the level of the weapon and sets
	new stats and properties*/
	void AddXP(int32 XpToAdd);

	
	inline float GetDamageOnCurrentLevel() { return DamageOnCurrentLevel; }

	FORCEINLINE UParticleSystem* GetParticleSystem() { return ParticleSystem; }

};