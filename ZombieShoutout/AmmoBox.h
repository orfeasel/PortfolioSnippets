// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pickupable.h"
#include "AmmoBox.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Ordinary,
	Incendiary,
	Frost
};


UCLASS()
class ROGUELIKESURVIVAL_API AAmmoBox : public APickupable
{
	GENERATED_BODY()
	
private:
	/*The total amount of bullets that this ammobox contains
	which is determined by the lower and the upper limit of ammo*/
	int32 AmountOfTotalBullets;

protected:
	UFUNCTION()
	virtual void ActorOverlaped(AActor* Other) override;

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**********************Ammo***********************/

	/*The ammo type which is inside the current ammobox*/
	UPROPERTY(EditAnywhere, Category = "Ammo")
	EAmmoType AmmoType;

	/*The lowest possible amount of bullets tha the character can pickup*/
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 LowerAmmoLimit;

	/*The highest possible amount of bullets that the character can pickup*/
	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 UpperAmmoLimit;

	/*The material of the box when it contains frost ammo*/
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	UMaterialInterface* FrostAmmoBoxMaterial;

	/*The material of the box when it contains incendiary ammo*/
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	UMaterialInterface* IncendiaryAmmoBoxMaterial;

	/*The frost ammo box particle emitter template*/
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	UParticleSystem* FrostAmmoBoxEmmiterTemplate;

	/*The incendiary ammo box particle emitter template*/
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	UParticleSystem* IncendiaryAmmoBoxEmmiterTemplate;

	/*Determines the right material for the current ammo box based on the AmmoType it provides*/
	void SetAmmoBoxMaterialAndParticleEmitter();
	
};
