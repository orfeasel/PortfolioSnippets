// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Public/Weapons/Weapon.h"
#include "Knife.generated.h"

/**
 * 
 */


UCLASS()
class RNC_API AKnife : public AWeapon
{
	GENERATED_BODY()

	
public:

	AKnife();

	/*Returns hit actors*/
	virtual bool GetHitActors(TArray<FHitResult> &WeaponHitActors) override;

};