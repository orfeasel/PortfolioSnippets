// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "ArenaTrigger.h"
#include "RnCCharacter.h"
#include "Public/Environment/IntermissionNode.h"

//-------------------------------------------------------------------------------------
//Enable the boss fight
//-------------------------------------------------------------------------------------
void AArenaTrigger::OnOverlapedActor(AActor* OverlapedActor)
{
	ARnCCharacter* MainCharacter = Cast<ARnCCharacter>(OverlapedActor);
	if(MainCharacter && BossCharacterRef)
	{
		MainCharacter->EnableBossUI();

		//Spawn an appropriate controller for the boss and possess it.
		FActorSpawnParameters SpawnParams;
		ABossAIController* BossCon = GetWorld()->SpawnActor<ABossAIController>(BossConBP, SpawnParams);
		if(BossCon) BossCon->Possess(BossCharacterRef);


		MoveArenaDoors();
	}
}

void AArenaTrigger::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AArenaTrigger::OnOverlapedActor);

}
