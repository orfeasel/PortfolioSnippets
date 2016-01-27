// Fill out your copyright notice in the Description page of Project Settings.

#include "CavemanTestField.h"
#include "WolfCharacter.h"
#include "WolfAIController.h"
#include "EnemyStatsComponent.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AWolfCharacter::AWolfCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Sensing comp init.
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.f);
	PawnSensingComp->SightRadius = 2000;

	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComp"));
}

// Called when the game starts or when spawned
void AWolfCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Registering function senses
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AWolfCharacter::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AWolfCharacter::OnHearNoise);
	}
	//Registering the melee collider for the combat
	UBoxComponent* MeleeCollider = FindComponentByClass<class UBoxComponent>();
	if (MeleeCollider)
	{
		MeleeCollider->OnComponentBeginOverlap.AddDynamic(this, &AWolfCharacter::OnMeleeColliderOverlapStart);
		MeleeCollider->OnComponentEndOverlap.AddDynamic(this, &AWolfCharacter::OnMeleeColliderOverlapEnd);
	}

	//Register stats comp
	TArray<UEnemyStatsComponent*> CamComps;
	GetComponents<UEnemyStatsComponent>(CamComps);
	for (int32 i = 0; i < CamComps.Num(); i++)
	{
		Stats = CamComps[0];
	}

	//Set corresponding movement and attack speed
	if (Stats)
	{
		CharacterMovement->MaxWalkSpeed = Stats->Speed;
		MeleeHitCD = Stats->AttackSpeed;
	}


	//Register Anim instance
	MyAnimInstance = Cast<UWolfAnimInstance>(GetMesh()->GetAnimInstance());

}

// Called every frame
void AWolfCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	AWolfAIController* WolfController = Cast<AWolfAIController>(GetController());
	//If wolf has a target he must always face him
	if (WolfController)
	{
		//Wolf must always face his target
		if (bHasSensedTarget && !WolfController->IsFollowingAPath())
		{
			AActor* CurrentTarget = Cast<AActor>(WolfController->GetSensedTarget());
			if (CurrentTarget)
			{
				FRotator Rotation = (CurrentTarget->GetActorLocation() - GetActorLocation()).Rotation();
				//Set the roation of the wolf to a fixed pitch
				Rotation.Pitch = 0;
				SetActorRotation(Rotation);
			}
			if (MyAnimInstance->bCanCauseDamageNow)
			{
				PerformMeleeHit();
			}
		}


		AMyCharacter_FirstTry* Caveman = Cast<AMyCharacter_FirstTry>(WolfController->GetSensedTarget());
		//Check if current target is an "alive" caveman
		if (Caveman)
		{
			if (Caveman->Health_Current <= 0)
			{
				GLog->Log("Senses have timed-out!");
				bHasSensedTarget = false;
				WolfController->SetSensedTarget(nullptr);
				return;
			}
		}
		//Check if senses have timed-out
		if (bHasSensedTarget && (GetWorld()->TimeSeconds > 2) && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut)
		{
			GLog->Log("Senses have timed-out!");
			bHasSensedTarget = false;
			WolfController->SetSensedTarget(nullptr);
			PreviousSensedTarget = nullptr;
		}
	}
	
	//GLog->Log("My state is:" + FString::FromInt((uint8)MyAnimInstance->CurrentState));
	UpdateCorrespondingAnimation();
}

void AWolfCharacter::OnSeePlayer(APawn* Pawn)
{
	//Called when the wolf sees the player
	AWolfAIController* AIController = Cast<AWolfAIController>(GetController());
	AMyCharacter_FirstTry* SensedPawn = Cast<AMyCharacter_FirstTry>(Pawn);
	EnableHunting(SensedPawn);

}

void AWolfCharacter::UpdateCorrespondingAnimation()
{
	AWolfAIController* AIController = Cast<AWolfAIController>(GetController());
	if (AIController)
	{
		AActor* CurrentEnemy = AIController->GetSensedTarget();

		if (AIController->IsFollowingAPath())
		{
			MyAnimInstance->CurrentState = EWolfStates::Run;
			return;
		}
		//If you have combat stance let the AI set the corresponding animation
		else if (MyAnimInstance->CurrentState == EWolfStates::CombatStance)
		{
			return;
		}
		else
		{
			MyAnimInstance->CurrentState = EWolfStates::Idle;

		}
	}
}

void AWolfCharacter::GetHit(APawn* ActorWhoPerformedTheHit)
{
	AWolfAIController* AIController = Cast<AWolfAIController>(GetController());
	if (AIController)
	{
		//Play the animation
		MyAnimInstance->CurrentState = EWolfStates::GetHurt;

		//Set the sensed target as the target who hit the wolf.
		if (ActorWhoPerformedTheHit)
		{
			AIController->SetSensedTarget(ActorWhoPerformedTheHit);
		}
	}
}

void AWolfCharacter::PerformMeleeHit()
{
	if (CanPerformAttack())
	{
		UEnemyStatsComponent* StatComp = Cast<UEnemyStatsComponent>(GetComponentByClass(UEnemyStatsComponent::StaticClass()));
		AWolfAIController* Controller = Cast<AWolfAIController>(GetController());

		if (StatComp && Controller)
		{
			
			AMyCharacter_FirstTry* Target = Cast<AMyCharacter_FirstTry>(Controller->GetSensedTarget());
			if (Target)
			{
				//GLog->Log("TimeSeconds: " + FString::SanitizeFloat(GetWorld()->TimeSeconds) + ", LastHitTime: " + FString::SanitizeFloat(LastHitTime));
				if (bIsInMeleeRange)
				{
					
					Target->TakeDamageFromEnemy(StatComp->CauseDamage());
				}
				LastHitTime = GetWorld()->TimeSeconds;
			}
		}
	}

}

void AWolfCharacter::OnMeleeColliderOverlapStart(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMyCharacter_FirstTry>(OtherActor))
	{
		bIsInMeleeRange = true;
	}
}

void AWolfCharacter::OnMeleeColliderOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMyCharacter_FirstTry>(OtherActor))
	{
		bIsInMeleeRange = false;
	}
}

bool AWolfCharacter::CanPerformAttack()
{
	//Takes into consideration the melee hit cd and returns true if the wolf can attack.
	UEnemyStatsComponent* StatComp = Cast<UEnemyStatsComponent>(GetComponentByClass(UEnemyStatsComponent::StaticClass()));
	if ((GetWorld()->TimeSeconds - LastHitTime) >= MeleeHitCD && (StatComp->CurrentHealth > 0))
	{
		return true;
	}
	return false;

}

void AWolfCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	//If both the wolf and the caveman is alive enable wolf's hunting status
	if (Stats->CurrentHealth<=0)
	{
		return;
	}

	AMyCharacter_FirstTry* Caveman = Cast<AMyCharacter_FirstTry>(PawnInstigator->GetController()->GetCharacter());
	AWolfAIController* Controller = Cast<AWolfAIController>(GetController());
	if (Caveman && (Caveman->Health_Current>0) && Controller)
	{
		EnableHunting(Caveman);
	}

}

void AWolfCharacter::EnableHunting(AMyCharacter_FirstTry* SensedPawn)
{
	//Chases the caveman and stores the last seen time in case we need to time-out the hearing sense
	AWolfAIController* AIController = Cast<AWolfAIController>(GetController());
	if (AIController && SensedPawn && SensedPawn->Health_Current >= 0)
	{
		AWolfCharacter* Wolf = Cast<AWolfCharacter>(AIController->GetCharacter());
		if (!SensedPawn->bIsInCombat && Wolf && Wolf->Stats->CurrentHealth > 0)
		{
			SensedPawn->SetCombatStatus(true);
		}

		if (MyAnimInstance && SensedPawn != PreviousSensedTarget)
		{
			MyAnimInstance->CurrentState = EWolfStates::Howl;
			//Enable firebrand
			SensedPawn->SetCombatStatus(true);
		}

		AIController->SetSensedTarget(SensedPawn);
		PreviousSensedTarget = SensedPawn;

		LastSeenTime = GetWorld()->GetTimeSeconds();
		bHasSensedTarget = true;
		//GLog->Log("Wolf's got a target!");
	}
}