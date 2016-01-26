// Fill out your copyright notice in the Description page of Project Settings.

#include "RoguelikeSurvival.h"
#include "FirstAidKit.h"



AFirstAidKit::AFirstAidKit()
{
	//Default stats
	HealthBonus = 30.f;
	bIsOvertimeHeal = false;
	OvertimeHealDuration = 5.f;
	TickOfOvertimeHeal = 1.f;
}

void AFirstAidKit::BeginPlay()
{
	Super::BeginPlay();

	//Fixing the rotation values
	StaticMesh->SetRelativeScale3D(FVector(4.5, 4.5, 4.5f));
	FRotator Rot = FRotator(90.f, 0, 0);
	StaticMesh->SetWorldRotation(Rot);

	ParticleEmitter->SetRelativeRotation(FRotator(-90.f,0,0));
	ParticleEmitter->SetRelativeLocation(FVector(-10, 0, 0));
	


	RotatingMovementComponent->RotationRate = FRotator(0, 0, 180.f);

	if (bIsOvertimeHeal)
	{
		//Initializing the value in order to determine how many loops the apply overtime heal will execute
		HealthAmountLeftToApply = HealthBonus;
	}

	UpdateMaterial();
}

void AFirstAidKit::UpdateMaterial()
{
	if(bIsOvertimeHeal)
	{
		StaticMesh->SetMaterial(0, OvertimeHealthMaterial);
		//Initializing the value in order to determine how many loops the apply overtime heal will execute
		HealthAmountLeftToApply = HealthBonus;
	}
}

void AFirstAidKit::ActorOverlaped(AActor* Other)
{
	ARoguelikeChar* Character = Cast<ARoguelikeChar>(Other);
	if (Character)
	{
		//Play Powerup SFX
		AudioComp->Activate();
		AudioComp->Play();
		
		OurCharacter = Character;

		if (!bIsOvertimeHeal && !this->IsPendingKill())
		{
			Character->IncreaseHealth(HealthBonus);
			GLog->Log("You took a first aid kit");
			//AudioComp->Stop();
			//Destroy();
			this->Kill();
		}
		else
		{
			if (OvertimeHealDuration>0)
			{
				FTimerHandle TimerHandle;

				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFirstAidKit::ApplyOvertimeHeal, TickOfOvertimeHeal, true);
				//Disable the collision in order to avoid future timer activation
				StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				StaticMesh->SetVisibility(false);
				//AudioComp->Stop();
			}
		}
	}
}

void AFirstAidKit::ApplyOvertimeHeal()
{
	float HealthAmountForEachLoop = HealthBonus / OvertimeHealDuration;

	HealthAmountLeftToApply -= HealthAmountForEachLoop;

	OurCharacter->IncreaseHealth(HealthAmountForEachLoop);
	GLog->Log("Applied over time heal:" + FString::SanitizeFloat(HealthAmountForEachLoop));

	if (HealthAmountLeftToApply <= 0)
	{
		GLog->Log("Destroyed overtime heal");
		this->Kill();
		//Destroy();
	}
}