// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "ActorSpawner.h"

//-------------------------------------------------------------------------------------
//Spawns the associated actor
//-------------------------------------------------------------------------------------
void AActorSpawner::SpawnActor()
{
	FActorSpawnParameters SpawnParams;
	switch (ActorToSpawn)
	{
		case ETypeOfActor::AmmoBox:
		{
			GetWorld()->SpawnActor<ADestructibleBox>(DestructibleBoxBP, GetTransform(), SpawnParams);
			break;
		}
		case ETypeOfActor::HealthPickup:
		{
			AHealthPickup* Pickup = GetWorld()->SpawnActor<AHealthPickup>(HealthPickupBP, GetTransform(), SpawnParams);
			if(Pickup) 
				Pickup->SetActorScale3D(FVector(HealthPickupsMultiplier));
			break;
		}
		case ETypeOfActor::AIMinion:
		{
			AMeleeEnemyCharacter* Enemy=GetWorld()->SpawnActor<AMeleeEnemyCharacter>(MeleeEnemyBP, GetTransform(), SpawnParams);
			if (Enemy)
				Enemy->EnableHunting();
			break;
		}
	}
}
