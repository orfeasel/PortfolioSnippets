// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "Public/Weapons/Knife.h"
#include "DrawDebugHelpers.h"
#include "Public/AI/BaseEnemyCharacter.h"

AKnife::AKnife()
{
	bIsMelee = true;
	CurrentAmmo = 0;

	CurrentComboMove = 1;
	WeaponType = EWeaponType::Knife;
	bIsUnlocked = true;
}

//---------------------------------------------------------------------------------------------------
//Returns hit actors if the knife is raycasting
//---------------------------------------------------------------------------------------------------
bool AKnife::GetHitActors(TArray<FHitResult> &WeaponHitActors)
{
	if (bIsRaycasting && WeaponSM)
	{
		//We will raycast in a single line
		FCollisionShape CollisionShape;
		CollisionShape.ShapeType = ECollisionShape::Line;

		//Search for pawns and destructibles only
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

		//Exclude our character from the raycast
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(WeaponSM->GetAttachmentRootActor());
		
		//Default quaternion
		FQuat q;

		//Determine if we hit anything
		bool CollisionResult = GetWorld()->SweepMultiByObjectType(WeaponHitActors, WeaponSM->GetSocketLocation("LineRaycastStart"),
			WeaponSM->GetSocketLocation("LineRaycastEnd"), q, ObjectQueryParams, CollisionShape, CollisionQueryParams);

		FDamageEvent DamageEvent;

		//Apply damage to all the overlaping actors
		for(auto It : WeaponHitActors)
		{
			AActor* HitActor = It.GetActor();

			if (HitActor)
				HitActor->TakeDamage(CurrentWeaponStats.GetDamageOnCurrentLevel()*CurrentComboMove, DamageEvent, GetInstigatorController(), GetOwner());

			ABaseEnemyCharacter* Enemy = Cast<ABaseEnemyCharacter>(HitActor);
			if (Enemy && !Enemy->IsAlive()) CurrentWeaponStats.AddXP(Enemy->GetXpReward());
		}

		//Raycasting...optional
		//DrawDebugLine(GetWorld(), WeaponSM->GetSocketLocation("LineRaycastStart"), WeaponSM->GetSocketLocation("LineRaycastEnd"), FColor::Red, false, 1.f, 0, 2);

		return CollisionResult;
		
	}
	else
	{
		//GLog->Log("Raycast off");
		return false;
	}


}