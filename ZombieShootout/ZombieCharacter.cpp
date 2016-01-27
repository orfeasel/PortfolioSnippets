// Fill out your copyright notice in the Description page of Project Settings.

#include "RoguelikeSurvival.h"
#include "ZombieCharacter.h"
#include "ZombieController.h"
#include "PlayerDamageType.h"
#include "RoguelikeChar.h"
#include "ZombieAnimInstance.h"
//#include "DrawDebugHelpers.h"
#include "RoguelikeGameInstance.h"


// Sets default values
AZombieCharacter::AZombieCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing stats and components.

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("Audio"));
	AudioComponent->AttachTo(GetRootComponent());


	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleSystemComponent->bAutoActivate = false;

	ParticleSystemComponent->AttachTo(GetRootComponent());
	

	//Combat stats
	MinHealth = 100.f;
	MaxHealth = 300.f;

	MinSpeed = 150.f;
	MaxSpeed = 250.f;

	MinDamage = 5.f;
	MaxDamage = 15.f;

	DotTickInSeconds = 1.f;
	SlowPercentage = 0.5f;
	SlowEffectDuration = 3.f;
	Damage = 15.f;

	MeleeDistanceThreshold = 150.f;
}

// Called when the game starts or when spawned
void AZombieCharacter::BeginPlay()
{
	Super::BeginPlay();

	//ParticleSystemComponent->SetWorldLocation(GetActorLocation());
	ParticleSystemComponent->SetRelativeLocation(FVector(90, 0, 40));
	AudioComponent->SetRelativeLocation(FVector(90, 0, 40));
	
	//Stats initialization
	StatsInit(MinHealth, MaxHealth, MinDamage, MaxDamage);
	CurrentSpeed = FMath::RandRange(MinSpeed, MaxSpeed);

	bHasDot = false;
	bHasSlow = false;

	InitialMaxWalkSpeed = CurrentSpeed;
	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;

	//Animations initialization
	ZAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	ZAnimInstance->Speed = CurrentSpeed;

	//Setting up the player reference for future use
	OurCharacter = Cast<AZombieController>(GetController())->GetTarget();

	if(AlphaReductionCurveFloat && DeathMaterialInstance)
	{
		//Binds the functions for the progress and the finished function on the timeline
		FOnTimelineFloat TimelineFunctionProgress;

		TimelineFunctionProgress.BindUFunction(this, FName("HandleTimelineProgress"));

		FOnTimelineEvent CompletedTimelineEvent;
		CompletedTimelineEvent.BindUFunction(this, FName("Destroy"));
		
		//Binding...
		AlphaReductionTimeline.AddInterpFloat(AlphaReductionCurveFloat, TimelineFunctionProgress);
		AlphaReductionTimeline.SetTimelineFinishedFunc(CompletedTimelineEvent);
	}
}

// Called every frame
void AZombieCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	GetCharacterMovement()->MaxWalkSpeed = InitialMaxWalkSpeed;
	ZAnimInstance->Speed = InitialMaxWalkSpeed;

	//If the zombie is within the melee range cancel any movement animation so it's more realistic
	float Distance = FVector::Dist(GetActorLocation(),OurCharacter->GetActorLocation());
	if (Distance<= MeleeDistanceThreshold)
	{
		ZAnimInstance->Speed = 0;
	}

	//Enable the attack raycast sphere in zombies hand if the animation is valid
	if (ZAnimInstance->bEligibleForAttack)
	{
		Attack();
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = InitialMaxWalkSpeed;
		}
	}

	//Updates the alpha value if the zombie is dead to achieve over-time fade out
	if(!IsAlive())
	{
		//Ticks the alpha reduction timeline
		AlphaReductionTimeline.TickTimeline(DeltaTime);
	}

}

float AZombieCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//Determines the damage amount and type in order to apply it to the zombie
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	OurCharacter = Cast<ARoguelikeChar>(DamageCauser);

	HandleDamage(DamageAmount, DamageEvent);

	return ActualDamage;
}

void AZombieCharacter::HandleDamage(float DamageAmount, FDamageEvent DamageEvent)
{
	//Handles the given damage amount and effect
	//Makes sures to apply the right debuff (if any - based on the bullet type the player used) and damage to the zombie
	//Takes into consideration the active debuff in order to deactivate it if necessary (ie dot cancels slow effect and vice versa)

	if(IsAlive())
	{

		//Determing the damage type based on the bullet that the player used
		//ie frost damage if frost ammo is equipped
		UPlayerDamageType* DamageType = Cast<UPlayerDamageType>(DamageEvent.DamageTypeClass.GetDefaultObject());
		switch (DamageType->PlayerDamageType)
		{
			case EPlayerDamageType::Dot:
			{
				DisableSlowEffect();
				ApplyDotEffect(DamageAmount);
				break;
			}
			case EPlayerDamageType::Slow:
			{
				DisableDotEffectIfNecessary(true);
				ApplyDamage(DamageAmount);
				ApplySlowEffect();
				break;
			}
			default:
			{
				ApplyDamage(DamageAmount);
				break;
			}
		}

		if (!IsAlive())
		{
			//Stop logic of the AI
			Die();
			
		}
	}
}

void AZombieCharacter::ApplyDamage(float DamageAmount)
{
	//Applies damage to the zombie

	//Updating the animation of the zombie 
	ZAnimInstance->GetHurt();
	if (bHasDot)
	{
		DisableDotEffectIfNecessary();
	}
	CurrentHealth -= DamageAmount;
	if (!IsAlive())
	{
		Die();
	}
	GLog->Log("Zombie has took damage, current hp:" + FString::SanitizeFloat(CurrentHealth));
}

void AZombieCharacter::DisableDotEffectIfNecessary(bool ForceStop)
{
	//Disables the dot effect is necessary (ie dot timer has been depleted)
	//If ForceStop is active it forces the dot effect to stop even if the dot timer is still active
	UWorld* World = GetWorld();
	//Forced stop of dot effect
	if (ForceStop && World && OurCharacter && World->GetTimerManager().IsTimerActive(DotTimerHandle))
	{
		GLog->Log("Stopping Dot Effect...");
		World->GetTimerManager().ClearTimer(DotTimerHandle);

		ParticleSystemComponent->Deactivate();
		//ZombieDebuffComp->SetFireParticleEnabled(false);

		bHasDot = false;

	}
	else
	{	//Disable effect due to time out
		if (World && OurCharacter &&
			World->GetTimerManager().IsTimerActive(DotTimerHandle) && 
			World->GetTimeSeconds() - DotApplicationTime>=OurCharacter->DotDuration)
		{
			World->GetTimerManager().ClearTimer(DotTimerHandle);
		
			ParticleSystemComponent->Deactivate();
			//ZombieDebuffComp->SetFireParticleEnabled(false);

			bHasDot = false;
		}
	}

	
	//ParticleSystemComponent->Deactivate();
	
}

void AZombieCharacter::ApplyDotEffect(float DamageAmount)
{
	//Applies the damage over time effect on zombie and sets up the corresponding particle template
	//Moreover, initializes the dot timer handle so certain damage is applied over-time.
	if (!bHasDot)
	{
		AZombieController* ZCon = Cast<AZombieController>(GetController());
		if (ZCon)
		{
			
			//Calculating the tick damage that we're going to apply in the zombie
			//based on the given damage amount.
			float DotTimer = OurCharacter->DotDuration;

			float TickDamage = DamageAmount / DotTimer;

			//The delegate that is binded to the apply damage function
			FTimerDelegate Delegate;
			Delegate.BindUFunction<AZombieCharacter, float>(this, FName("ApplyDamage"), TickDamage);

			//Enabling the dot effect..
			UWorld* World = GetWorld();
			if (World && OurCharacter)
			{
				World->GetTimerManager().SetTimer(DotTimerHandle, Delegate, DotTickInSeconds, true);
				DotApplicationTime = World->GetTimeSeconds();
				GLog->Log("Dot application seconds:" + FString::SanitizeFloat(DotApplicationTime));
				bHasDot = true;

				//Resets the particle system template
				if(ParticleSystemComponent)
				{
					ParticleSystemComponent->SetTemplate(FireEffectParticleTemplate);
					ParticleSystemComponent->Activate(true);
					//Activate the fire particles
					/*ParticleSystemComponent->Deactivate();
					ParticleSystemComponent->Template = FireEffectParticleTemplate;
					ParticleSystemComponent->Activate(true);*/
					//ZombieDebuffComp->SetFireParticleEnabled(true);
					
				}
				
			}
		}
	}
}

void AZombieCharacter::ApplySlowEffect()
{
	//Applies the slow effect on zombie and sets up the slow timer
	//as well as the right particle template

	if (!bHasSlow)
	{
		DisableDotEffectIfNecessary(true);
		bHasSlow = true;

		float SlowSpeed = InitialMaxWalkSpeed * (1 - SlowPercentage);

		//Activating slow
		GetCharacterMovement()->MaxWalkSpeed = SlowSpeed;

		//Reseting the slow particle template
		ParticleSystemComponent->SetTemplate(SlowEffectParticleTemplate);
		ParticleSystemComponent->Activate(true);

		GLog->Log("Applied slow effect on zombie.");

		//Initializing the slow timer handle
		UWorld* World = GetWorld();
		if (World && OurCharacter)
		{
			World->GetTimerManager().SetTimer(SlowTimerHandle, this, &AZombieCharacter::DisableSlowEffect, SlowEffectDuration);
		}
	}
}

void AZombieCharacter::DisableSlowEffect()
{
	//Disables the slow effect on the zombie and applies its default movement speed
	bHasSlow = false;
	
	GetCharacterMovement()->MaxWalkSpeed = InitialMaxWalkSpeed;
	GLog->Log("Disabled slow effect on zombie.");

	UWorld* World=GetWorld();
	if (World)
	{
		//Clearing the slow timer so the effect can be applied again
		World->GetTimerManager().ClearTimer(SlowTimerHandle);
	}

	//Turning of the particle system
	ParticleSystemComponent->Deactivate();
}

void AZombieCharacter::StatsInit(float MinHealth, float MaxHealth, float MinDamage, float MaxDamage)
{
	//Initializes the stats of the zombie based on the given range of values
	CurrentHealth = FMath::RandRange(MinHealth, MaxHealth);
	Damage = FMath::RandRange(MinDamage, MaxDamage);
}

void AZombieCharacter::Attack()
{
	//This function is used only when it's permitted by the zombie's animation instance
	//It creates a raycast in a sphere shape and checks for possible hits
	//If the hits contain our player it makes sure it applies damage to him

	//Setting up the start and end location of the raycast
	FVector StartLocation = GetMesh()->GetSocketLocation(FName("MeleeStartSocket"));
	FVector EndLocation = GetMesh()->GetSocketLocation(FName("MeleeEndSocket"));


	//Raycasting in a sphere to detect collisions
	TArray<FHitResult> HitResults;

	//Setting up the shape of the raycast
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(AttackRaycastRadius);

	//Object query parameters
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AllDynamicObjects;

	//Handling ignored actors
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	UWorld* World = GetWorld();
	if (World && ZAnimInstance->bEligibleForAttack)
	{
		//Raycasting...
		bool bHit = World->SweepMultiByObjectType(HitResults, StartLocation, EndLocation, FQuat::Identity, ObjectQueryParams, CollisionShape, QueryParams);

		//Raycast visualization
		/*FVector Center = ((EndLocation - StartLocation) / 2) + StartLocation;
		DrawDebugSphere(World, Center, AttackRaycastRadius, 20, FColor::Green, false, 2.f);*/

		//Checking for possible hits
		if (bHit)
		{
			for (auto It = HitResults.CreateIterator(); It; It++)
			{
				ARoguelikeChar* Char = Cast<ARoguelikeChar>(It->GetActor());
				if (Char && ZAnimInstance && GetCharacterMovement())
				{
					//Calling the attack function from character
					Char->TakeDamageFromZombie(Damage);
					
					//Closing the flag which checks for the attack function
					ZAnimInstance->bEligibleForAttack = false;

					//Updating with new movement speed
					GetCharacterMovement()->MaxWalkSpeed = InitialMaxWalkSpeed;
					ZAnimInstance->Speed = InitialMaxWalkSpeed;
					break;
				}
			}
		}
	}

}

void AZombieCharacter::Die()
{
	//Disables the logic of the zombie and gradually destroys this Actor

	AZombieController* ZCon = Cast<AZombieController>(GetController());
	if (ZCon)
	{
		//Increasing the kills and wave count if necessary
		OurCharacter->Kills++;
		URoguelikeGameInstance* GameInstance = Cast<URoguelikeGameInstance>(GetGameInstance());
		GameInstance->IncreaseWaveCountIfNeeded();
		OurCharacter->PlayerController->UpdateUI();

		//Stopping the AI logic
		ZCon->UnPossess();

		AlphaChannelReduction();
		//AlphaReductionTimeline.PlayFromStart();

		//Handling the last animations
		ZAnimInstance->bIsDead = true;
		ZAnimInstance->Speed = 0;

		AudioComponent->Stop();

		//Disabling the collision to avoid false kill count!
		UCapsuleComponent* RootComp = Cast<UCapsuleComponent>(GetRootComponent());
		RootComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
}

void AZombieCharacter::HandleTimelineProgress(float AlphaReduction)
{
	//Gradually reducing the Alpha value of the material of the zombie
	//if(DeathMaterialInstance)
	//{
	//	//GLog->Log("Timeline tick");
	//	//GetMesh()->CreateAndSetMaterialInstanceDynamicFromMaterial(0, DeathMaterialInstance);
	//	GetMesh()->CreateDynamicMaterialInstance(0, DeathMaterialInstance);
	//	GLog->Log("Alpha reduction:" + FString::SanitizeFloat(AlphaReduction));

	//	//DeathMaterialInstance->OverrideScalarParameterDefault(FName("AlphaReduction"), AlphaReduction, true, ERHIFeatureLevel::Num);
	//	TArray<FScalarParameterValue> ZombieMatInstScalarParams;
	//	ZombieMatInstScalarParams = DeathMaterialInstance->ScalarParameterValues;
	//	
	//	for (auto It = ZombieMatInstScalarParams.CreateIterator(); It;It++)
	//	{
	//		if((*It).ParameterName==FName("AlphaReduction"))
	//		{
	//			(*It).ParameterValue = AlphaReduction;
	//			break;
	//		}
	//	}

	//	FScalarParameterValue ZombieInstance;
	//	ZombieInstance.ParameterName = FName("AlphaReduction");
	//	
	//	//int32 AlphaReductionPropertyIndex = ZombieMatInstScalarParams.Find(ZombieInstance.ParameterName);

	//	auto* Parameter=ZombieMatInstScalarParams.FindByPredicate( [&] (int32 i) {return ZombieMatInstScalarParams[i].ParameterName == FName("AlphaReduction"); });
	//	Parameter->ParameterValue = AlphaReduction;
	//	

	//	//ZombieMatInstScalarParams[AlphaReductionPropertyIndex].ParameterValue = AlphaReduction;

	//}
}

//void AZombieCharacter::KillZombie()
//{
//	Destroy();
//}
