// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "WeaponStats.h"

//-------------------------------------------------------------------------------------
//Updates the stats of the weapon based on the attached data table
//-------------------------------------------------------------------------------------
void FWeaponStats::UpdateStats(int32 DataTableRow)
{
	FName RowName = FName(*FString::FromInt(DataTableRow));
	FWeaponStats* NewStats = DataTable->FindRow<FWeaponStats>(RowName, FString(""));
	if (NewStats)
	{
		XpToNextLevel = NewStats->XpToNextLevel;
		DamageOnCurrentLevel = NewStats->DamageOnCurrentLevel;

		//Load the particle if valid
		if (!NewStats->ParticleSystemOnCurrentLevel.IsNull()) ParticleSystem = NewStats->ParticleSystemOnCurrentLevel.LoadSynchronous();
	}
}

FWeaponStats::FWeaponStats()
{
	//Starting level
	CurrentLevel = 1;
	CurrentXP = 0;
}

//-------------------------------------------------------------------------------------
//Associates a data table for the player stats
//-------------------------------------------------------------------------------------
void FWeaponStats::SetDataTable(UDataTable* WeaponDataTable)
{
	DataTable = WeaponDataTable;

	UpdateStats(CurrentLevel);
	
}

//-------------------------------------------------------------------------------------
//Adds an xp reward into the weapon
//-------------------------------------------------------------------------------------
void FWeaponStats::AddXP(int32 XpToAdd)
{
	CurrentXP += XpToAdd;
	if (CurrentXP >= XpToNextLevel && DataTable)
	{
		CurrentLevel++;
		UpdateStats(CurrentLevel);
	}
}
