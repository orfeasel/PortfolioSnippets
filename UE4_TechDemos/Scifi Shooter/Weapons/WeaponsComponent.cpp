// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "WeaponsComponent.h"
#include "RnCCharacter.h"
#include "Public/Weapons/Knife.h"

//------------------------------------------------------------------------------------------------------------
//Returns a reference of the desired weapon
//------------------------------------------------------------------------------------------------------------
AWeapon* UWeaponsComponent::GetWeaponBasedOnType(EWeaponType WeaponType)
{
	for (auto It = WeaponsArray.CreateConstIterator(); It;It++)
	{
		if((*It) && (*It)->GetWeaponType()==WeaponType)
		{
			return *It;
		}
	}
	//didn't find a valid reference
	return nullptr;
}

// Sets default values for this component's properties
UWeaponsComponent::UWeaponsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWeaponsComponent::BeginPlay()
{
	Super::BeginPlay();


	ARnCCharacter* MainChar = Cast<ARnCCharacter>(GetOwner());

	if(MainChar)
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.Owner = MainChar;

		UWorld* World = GetWorld();

		//Spawn all the weapons
		for (auto It = AvailableWeaponBlueprintReferences.CreateIterator(); It; It++)
		{
			AWeapon* WeaponRef = World->SpawnActor<AWeapon>(It->GetDefaultObject()->GetClass(), MainChar->GetTransform(), ActorSpawnParams);

			WeaponsArray.Add(WeaponRef);

			WeaponRef->SetActorHiddenInGame(true);

			WeaponRef->AttachRootComponentTo(MainChar->GetMesh(), WeaponRef->GetCharacterSocket(), EAttachLocation::SnapToTargetIncludingScale);

		}
	}
}

AWeapon* UWeaponsComponent::GetRandomWeaponReference(bool ShouldBeMelee)
{
	bool ContainsAnyDesiredWeapon = false;

	//first we iterate in order to find if there is any match at all
	for (auto It = WeaponsArray.CreateConstIterator(); It; It++)
	{
		if ((*It) && (*It)->IsMelee() == ShouldBeMelee)
		{
			ContainsAnyDesiredWeapon = true;
			break;
		}
	}

	if(ContainsAnyDesiredWeapon)
	{
		int32 RandomIndex;
		do
		{
			RandomIndex = FMath::RandRange(0, WeaponsArray.Num() - 1);
		} while (WeaponsArray[RandomIndex]->IsMelee() != ShouldBeMelee);

		return WeaponsArray[RandomIndex];
	}
	else return nullptr;
}

//------------------------------------------------------------------------------------------------------------
//Updates the equipped weapon of the character
//------------------------------------------------------------------------------------------------------------
void UWeaponsComponent::UpdateCharacterEquippedWeapon(EWeaponType WeaponToEquip)
{
	ARnCCharacter* Char = Cast<ARnCCharacter>(GetOwner());
	if (Char && Char->GetMovementComponent() && !Char->GetMovementComponent()->IsFalling())
	{
		//If the desired weapon is valid equip it and disable all the other weapons
		AWeapon* DesiredWeapon = GetWeaponBasedOnType(WeaponToEquip);
		if(DesiredWeapon && DesiredWeapon->IsUnlocked())
		{
			//Equip the new weapon
			DesiredWeapon->SetActorHiddenInGame(false);
			Char->SetNewEquippedWeapon(DesiredWeapon);

			//Disable all the others
			for (auto It = WeaponsArray.CreateIterator(); It;It++)
			{
				if((*It)!=DesiredWeapon)
				{
					(*It)->SetActorHiddenInGame(true);
				}
			}
		}
		
	}
}

//------------------------------------------------------------------------------------------------------------
//Increments the ammo of the desired weapon
//------------------------------------------------------------------------------------------------------------
void UWeaponsComponent::UpdateWeaponAmmo(EWeaponType WeaponType, int32 AmmoToAdd)
{
	//Find the desired weapon and increase its ammo
	AWeapon* DesiredWeapon = GetWeaponBasedOnType(WeaponType);

	if (DesiredWeapon) DesiredWeapon->IncreaseCurrentAmmo(AmmoToAdd);
}

//------------------------------------------------------------------------------------------------------------
//Unlocks the desired weapon
//------------------------------------------------------------------------------------------------------------
void UWeaponsComponent::UnlockWeapon(EWeaponType WeaponType)
{
	//Find the desired weapon and unlock it
	AWeapon* WeaponToUnlock = GetWeaponBasedOnType(WeaponType);

	if (WeaponToUnlock) WeaponToUnlock->Unlock();
}

void UWeaponsComponent::ResetComboMoveOnWeapon(EWeaponType WeaponType)
{
	//By convention in this game the only weapons that are affected by combo moves are melee weapons

	AWeapon* WeaponToReset = GetWeaponBasedOnType(WeaponType);

	if (WeaponToReset) WeaponToReset->ResetComboMoves();
}
