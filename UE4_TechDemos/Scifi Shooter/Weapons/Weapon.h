// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Public/Weapons/WeaponStats.h"
#include "Weapon.generated.h"

#define INITIAL_COMBO_MOVE_COUNT 1
#define MELEE_MAX_COMBO_MOVES 3

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Knife,
	Shotgun,
	Rifle
};

UCLASS(ABSTRACT)
//UCLASS()
class RNC_API AWeapon : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	/*Returns true if the weapon has a populated TArray of Actors that were hit - false otherwise
	Before called, SetRaycasting must be true.
	WARNING:Must be overriden in every weapon*/
	virtual bool GetHitActors(TArray<FHitResult> &WeaponHitActors) 
	{
		//This macro means that the function MUST be overriden!
		unimplemented(); 
		return false;
	};

	/*Enable this in order for the weapon to raycast for possible hits*/
	void SetRaycasting(bool RaycastStatus) { bIsRaycasting = RaycastStatus; }

	/*Returns true if the weapon is currently raycasting*/
	inline bool IsRaycasting() { return bIsRaycasting; }

	/*Returns true if the weapon is melee*/
	inline bool IsMelee() { return bIsMelee; }

	/*Returns the current ammo of the weapon - only for ranged weapons*/
	inline int32 GetCurrentAmmo() { return CurrentAmmo; }

	/*Returns the weapon type*/
	EWeaponType GetWeaponType() { return WeaponType; }

	/*Returns the total damage of the weapon based on the attached weapon stats*/
	float GetTotalDamageAmount();

	/*Returns the skeletal mesh of the weapon*/
	FORCEINLINE USkeletalMeshComponent* GetWeaponSkeletalMesh() { return WeaponSM; }

	/*Increases the current ammo*/
	void IncreaseCurrentAmmo(int32 AmmoToAdd);

	/*Returns the attached socket name*/
	FORCEINLINE FName GetCharacterSocket() { return SocketToAttach; }

	/*Unlocks the weapon*/
	void Unlock();

	/*Returns true if the weapon is unlocked - false otheriwse*/
	inline bool IsUnlocked() { return bIsUnlocked; }

	/*Increases the xp of the weapon*/
	void AddWeaponXP(int32 XpToAdd);

	/*Increases the combo move of the weapon*/
	void IncreaseComboMove();

	/*Resets the combo count*/
	inline void ResetComboMoves() { CurrentComboMove = 1; }

protected:

	/*True if the weapon is raycasting*/
	bool bIsRaycasting;

	/*True if the weapon is melee*/
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	bool bIsMelee;

	/*True if the weapon is unlocked*/
	bool bIsUnlocked;

	/*Current combo move only for melee weapons*/
	int32 CurrentComboMove;

	/*The weapon type*/
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	EWeaponType WeaponType;

	/*The weapon stats table responsible for this weapon*/
	UPROPERTY(EditAnywhere, Category = "Weapon")
	UDataTable* WeaponStatsDataTable;

	/*Instance of weapon stats*/
	FWeaponStats CurrentWeaponStats;

	/*Maximum ammo - only for ranged weapons*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int32 MaxAmmo;

	/*Current Ammo. Set this up as initial Ammo too*/
	UPROPERTY(EditAnywhere, Category="Weapon")
	int32 CurrentAmmo;

	/*The socket's name of the character's skeletal mesh that we weapon is going to get attached to*/
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName SocketToAttach;

	/*Skeletal mesh of the weapon*/
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	USkeletalMeshComponent* WeaponSM;

	
};
