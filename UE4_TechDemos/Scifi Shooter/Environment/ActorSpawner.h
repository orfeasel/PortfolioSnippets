// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TargetPoint.h"
#include "Public/Environment/DestructibleBox.h"
#include "Public/Environment/HealthPickup.h"
#include "Public/AI/MeleeEnemyCharacter.h"
#include "ActorSpawner.generated.h"

UCLASS()
class RNC_API AActorSpawner : public ATargetPoint
{
	GENERATED_BODY()


protected:

	/*My Health pickup sm is too small so I will multiply by a value I think will do for my tech demo*/
	UPROPERTY(EditAnywhere)
		float HealthPickupsMultiplier = 2.5f;

	//-------------------------------------------------------------------------------------
	//Associated Blueprints - used for spawn
	//-------------------------------------------------------------------------------------

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADestructibleBox> DestructibleBoxBP;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AHealthPickup> HealthPickupBP;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AMeleeEnemyCharacter> MeleeEnemyBP;

public:

	UENUM(BlueprintType)
	enum class ETypeOfActor : uint8
	{
		AmmoBox,
		HealthPickup,
		AIMinion
	};

	/*The spawn actor that will get spawned on the intermission phase from this particular instance of actor spawner*/
	UPROPERTY(EditAnywhere, Category = "ActorSpawn")
		ETypeOfActor ActorToSpawn;

	/*Spawns the associated actor*/
	void SpawnActor();
};
