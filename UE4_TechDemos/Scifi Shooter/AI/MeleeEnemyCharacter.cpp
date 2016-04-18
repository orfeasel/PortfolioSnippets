// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "Public/AI/MeleeEnemyCharacter.h"
#include "RnCCharacter.h"
#include "Public/AI/BaseAIController.h"
#include "DrawDebugHelpers.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	BaseDamageAmount = 20.f;
	AttackRaycastRadius = 15.f;
}

void AMeleeEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(AnimInstance && AnimInstance->GetDamageRaycastStatus())
	{
		FVector UpperSphereSocketLocation = GetMesh()->GetSocketLocation(UpperSphereSocket);
		FVector LowerSphereSocketLocation = GetMesh()->GetSocketLocation(LowerSphereSocket);

		FVector CenterOfRaycast = ((UpperSphereSocketLocation - LowerSphereSocketLocation)/2) + LowerSphereSocketLocation;

		//Visualizing collision sphere just for debug purposes
		//DrawDebugSphere(GetWorld(), CenterOfRaycast, AttackRaycastRadius, 10, FColor::Green, false, 1.f);

		const UWorld* World = GetWorld();
		if(World)
		{

			TArray<FHitResult> HitResults;

			//Setting up the collision shape
			FCollisionShape CollisionShape;
			CollisionShape.ShapeType = ECollisionShape::Sphere;
			CollisionShape.SetSphere(AttackRaycastRadius);

			//Search only for pawns
			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

			//Ignore ourselves
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(this);

			bool bHasHitActors = World->SweepMultiByObjectType(HitResults, LowerSphereSocketLocation, UpperSphereSocketLocation, FQuat::Identity, ObjectQueryParams, CollisionShape, CollisionQueryParams);

			if(bHasHitActors)
			{
				for (auto It = HitResults.CreateIterator(); It; It++)
				{
					ARnCCharacter* RncChar = Cast<ARnCCharacter>(It->GetActor());
					if(RncChar)
					{
						FDamageEvent DmgEvent;

						RncChar->TakeDamage(BaseDamageAmount, DmgEvent, GetController(), this);

						AnimInstance->SetDamageRaycastStatus(false);
					}
				}
			}
		}
	}

}

void AMeleeEnemyCharacter::Attack()
{
	//GLog->Log("Attacking...");
	AnimInstance->PlayAttackAnimation();
}

//-------------------------------------------------------------------------------------
//Cancels the patrol system and hunts down the player until either one is dead
//-------------------------------------------------------------------------------------
void AMeleeEnemyCharacter::EnableHunting()
{
	ARnCCharacter* Char = Cast<ARnCCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ABaseAIController* AICon = Cast<ABaseAIController>(GetController());

	if (Char && AICon) AICon->SetNewTarget(Char);

}
