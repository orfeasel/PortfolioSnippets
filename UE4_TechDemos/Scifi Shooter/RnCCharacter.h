// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Public/Weapons/Weapon.h"
#include "Public/Weapons/WeaponsComponent.h"
#include "Public/AnimInstances/MainCharacterAnimInstance.h"
#include "Public/Controllers/RnCPlayerController.h"
#include "Public/Environment/JumpNode.h"
#include "Public/Environment/ProjectileBounceArea.h"
#include "RnCCharacter.generated.h"

#define MAX_HEALTH_VALUE 100

UCLASS(config=Game)
class ARnCCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/*Holds a reference to the currently equipped weapon*/
	AWeapon* CurrentlyEquippedWeapon;

	/*Holds a reference to the anim instance of the player*/
	UMainCharacterAnimInstance* MainCharAnimInstance;

	/*Starts to fire a gun*/
	void Fire();

	/*Stops to fire a gun*/
	void StopFire();

	/*True if tha player is firing*/
	bool bIsFiring;

	/*The main character's controller reference. We store it to avoid castings in the future*/
	ARnCPlayerController* MainCharController;

	/*Creates a a new Weapon Equip Binding*/
	void CreateNewWeaponEquipBinding(FName ActionBinding, EWeaponType WeaponTypeToEquip);

	//------------------------------------------------------------------------------------
	//Jump nodes
	//------------------------------------------------------------------------------------

	/*Jump node ref*/
	AJumpNode* JumpNode;

	/*Performs the jump on the available node*/
	UFUNCTION()
	void PerformJumpOnNode();

	/*Creates a closed area around the character and enables the projectiles to bounce*/
	UFUNCTION()
	void CreateProjectileBounceArea();

	//------------------------------------------------------------------------------------
	//BattleCry
	//------------------------------------------------------------------------------------

	bool bShouldProjectilesBounce;

	bool bIsInBattleCry;

public:
	ARnCCharacter();

	void EnableBossUI();

	//------------------------------------------------------------------------------------
	//Combat
	//------------------------------------------------------------------------------------
	inline float GetHealthValue() { return Health; }

	/*Return true if the health value of the player is increased by HealhToAdd.
	False means the player is full of hp and should not pickup the healthkit*/
	bool IncreaseHealthBy(float HealthToAdd);

	int32 GetAmmoLeft();
	
	/*Perform a Melee Attack or update combo move if pressed at the exact right moment*/
	void PerformMeleeAttack();

	/*Equipes the given weapon and updates the UI*/
	void SetNewEquippedWeapon(AWeapon* WeaponToEquip);

	/*Enables the damage raycast of currently equiped melee weapon*/
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EnableWeaponRaycasting();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE bool IsAlive() { return Health > 0; }

	FORCEINLINE AWeapon* GetCurrentlyEquippedWeapon() { return CurrentlyEquippedWeapon; }

	/*True if a melee weapon is equipped*/
	bool IsMeleeWeaponEquipped();

	FORCEINLINE bool IsFiringAGun() { return bIsFiring; }

	/*Stores a reference of the available jump node*/
	void SetJumpNode(AJumpNode* JumpNode) { this->JumpNode = JumpNode; }

	inline bool GetProjectileBounceEffect() { return bShouldProjectilesBounce; }

	/*Disables battle cry flag*/
	UFUNCTION(BlueprintCallable, Category = "Animations")
	void DisableBattlecryAnimation();

	//-------------------------------------------------------------------------------------
	//BeginPlay and Tick
	//-------------------------------------------------------------------------------------
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//------------------------------------------------------------------------------------
	//Pickups
	//------------------------------------------------------------------------------------

	/*Increases the Ammo on the given weapon*/
	UFUNCTION(BlueprintCallable,Category="Pickup")
	void IncreaseAmmo(int32 AmmoToAdd, EWeaponType WeaponType);

	/*The character will pickup anything when his distance is <= PickupDistance*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup")
	float PickupDistance;
	
	/*Returns the weapons Component*/
	FORCEINLINE UWeaponsComponent* GetWeaponsComponent() { return WeaponsComp; }

	//--------------------------------------------------------------------------------------
	//Movement
	//--------------------------------------------------------------------------------------

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

#pragma region MovementFunctions

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/*Handles how fast the player and the camera rotate*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	float AutomaticRotationRate;

#pragma endregion Movement and Look Functions

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	/*The weapons component which contains all the available weapons of the character*/
	UPROPERTY(VisibleAnywhere)
	UWeaponsComponent* WeaponsComp;

	//-----------------------------------------------------------------------------------------------
	//Combat Related
	//-----------------------------------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float Health;

	//-----------------------------------------------------------------------------------------------
	//Bounce Area properties
	//-----------------------------------------------------------------------------------------------
	UPROPERTY(EditDefaultsOnly, Category="BounceArea")
	TSubclassOf<AProjectileBounceArea> BounceAreaBP;

	/*The distance in unreal units that the front and rear wall will be placed (relative to the player)*/
	UPROPERTY(EditAnywhere, Category = "BounceArea")
	float ForwardVectorMultiplier;

	/*The distance in unreal units that the right and left wall will be placed (relative to the player)*/
	UPROPERTY(EditAnywhere, Category = "BounceArea")
	float RightVectorMultiplier;

	/*The distance in unreal units that the ceiling will be placed (relative to the player)*/
	UPROPERTY(EditAnywhere, Category = "BounceArea")
	float UpVectorMultiplier;

	/*A unified scale multiplier for all the walls*/
	UPROPERTY(EditAnywhere, Category = "BounceArea")
	FVector UnifiedScaleMultiplier;

	/*The life time of the bounce area in seconds*/
	UPROPERTY(EditAnywhere, Category = "BounceArea")
	float LifeTimeOfBounceArea;

};

