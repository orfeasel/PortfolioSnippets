// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "BossCharacter.h"
#include "DrawDebugHelpers.h"
#include "Public/AI/BaseAIController.h"

void ABossCharacter::Tick(float DeltaSeconds)
{
	if(AnimInstance && AnimInstance->GetDamageRaycastStatus())
	{

		//Fix the rotation of the boss in order to hit the target if in range
		ABaseAIController* Controller = Cast<ABaseAIController>(GetController());
		if(Controller)
		{
			AActor* CurrentTarget = Cast<ACharacter>(Controller->GetCurrentTarget());

			if(CurrentTarget)
			{
				FRotator NewRotation = (CurrentTarget->GetActorLocation() - GetActorLocation()).Rotation();
				NewRotation.Pitch = 0;
				NewRotation.Yaw -= YawModifier;
				SetActorRotation(NewRotation);
			}
		}

		FVector UpperSocket = GetMesh()->GetSocketLocation(UpperHandSocket);
		FVector LowerSocket = GetMesh()->GetSocketLocation(LowerHandSocket);

		FCollisionShape CollisionShape;
		CollisionShape.ShapeType = ECollisionShape::Line;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		FQuat DefaultQuaternion;

		//DrawDebugLine(GetWorld(), UpperSocket, LowerSocket, FColor::Red, false, 1.f, 0, 2);

		TArray<FHitResult> HitActors;

		bool CollisionResult = GetWorld()->SweepMultiByObjectType(HitActors, UpperSocket, LowerSocket, DefaultQuaternion, ObjectQueryParams, CollisionShape, CollisionQueryParams);

		if(CollisionResult)
		{
			FDamageEvent dmgevent;
			for (auto It = HitActors.CreateConstIterator(); It; It++)
			{
				(*It).GetActor()->TakeDamage(BaseDamageAmount, dmgevent, GetController(), this);
			}
		}

	}
}

void ABossCharacter::Attack()
{
	//GetMovementComponent()->StopMovementImmediately();
	AnimInstance->PlayAttackAnimation();
}
