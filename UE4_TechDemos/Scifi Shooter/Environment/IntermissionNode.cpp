// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "IntermissionNode.h"
#include "Public/AI/BossAIController.h"

//-------------------------------------------------------------------------------------
//Of the boss reaches the intermission node enable the associated wall to block the
//the character and spawn all the pickups and AIs associates with this instance
//-------------------------------------------------------------------------------------
void AIntermissionNode::OnBoxOverlap(AActor* OverlapedActor, UPrimitiveComponent* OverlapedComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OverlapedActor)
	{
		ABossAIController* BossConRef = Cast<ABossAIController>(OverlapedActor->GetInstigatorController());

		if(BossConRef && BossConRef->GetCurrentIntermissionNode()==this && SMComp)
		{
			//This is the node that the boss has selected as an intermission node - "raise" the wall
			SMComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SMComp->SetVisibility(true);

			for (auto It = ActorSpawnerArray.CreateIterator(); It; It++)
			{
				(*It)->SpawnActor();
			}
		}
	}
}

//-------------------------------------------------------------------------------------
//When the boss exits the intermission phase disable the associated wall
//-------------------------------------------------------------------------------------
void AIntermissionNode::OnBoxEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		ABossAIController* BossConRef = Cast<ABossAIController>(OtherActor->GetInstigatorController());
		if(BossConRef && BossConRef->GetCurrentIntermissionNode()!=this)
		{
			//The boss is exiting from the intermission phase - "lower" the wall
			SMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SMComp->SetVisibility(false);
		}
	}
}

// Sets default values
AIntermissionNode::AIntermissionNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(FName("SceneComp"));

	SetRootComponent(SceneComp);

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("SMComp"));

	SMComp->AttachTo(SceneComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	BoxComp->AttachTo(SceneComp);
}

// Called when the game starts or when spawned
void AIntermissionNode::BeginPlay()
{
	Super::BeginPlay();

	//Initializing collisions and trigger events

	if(SMComp)
	{
		SMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SMComp->SetVisibility(false);
	}

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AIntermissionNode::OnBoxOverlap);
	
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AIntermissionNode::OnBoxEndOverlap);

}
