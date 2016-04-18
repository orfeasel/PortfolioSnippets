// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "Public/AI/BaseEnemyCharacter.h"


// Sets default values
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//If I forget to adjust the enemy's damage I will simply get one-shoted!
	BaseDamageAmount = 200000.f;

	Health = 100.f;
}

// Called when the game starts or when spawned
void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UBaseEnemyAnimInstance>(GetMesh()->GetAnimInstance());
}

float ABaseEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	if(FMath::Abs<float>(LastAttack-GetWorld()->GetRealTimeSeconds())>=SpamAttack)
	{
		Health -= DamageAmount;

		if (Health > 0)
		{
			AnimInstance->PlayGetHitAnimation();
		}
		else
		{
			AnimInstance->Die();
			SetActorEnableCollision(false);

			if (Controller) GetController()->UnPossess();

		}

		LastAttack = GetWorld()->GetRealTimeSeconds();
	}

	return Health;
}