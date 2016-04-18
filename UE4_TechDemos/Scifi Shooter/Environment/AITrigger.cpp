// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "AITrigger.h"
#include "RnCCharacter.h"
#include "Public/AI/MeleeEnemyCharacter.h"

//-------------------------------------------------------------------------------------
//The associated AI will be notified to chase down the player if he overlaps with this
//trigger
//-------------------------------------------------------------------------------------
void AAITrigger::OnTriggerOverlap(AActor* OverlapedActor)
{
	if (OverlapedActor->IsA<ARnCCharacter>())
	{
		TArray<AActor*> Enemies;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMeleeEnemyCharacter::StaticClass(), Enemies);

		for (auto It = Enemies.CreateIterator(); It;It++)
		{
			AMeleeEnemyCharacter* Enemy = Cast<AMeleeEnemyCharacter>(*It);
			if (Enemy) Enemy->EnableHunting();
		}

		GLog->Log("Notified ai");
	}
}

void AAITrigger::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AAITrigger::OnTriggerOverlap);
}
