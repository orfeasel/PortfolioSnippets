// Fill out your copyright notice in the Description page of Project Settings.

#include "CavemanTestField.h"
#include "MyCharacter_FirstTry.h"
#include "WolfCharacter.h"
#include "Trap.h"


// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Components init
	TrapStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapStaticMesh"));
	TrapStaticMesh->AttachTo(GetRootComponent());

	TrapCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("TrapCollider"));
	TrapCollider->AttachTo(TrapStaticMesh);
	TrapCollider->bGenerateOverlapEvents = true;
	
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	TrapCollider->OnComponentBeginOverlap.AddDynamic(this, &ATrap::TriggerTrap);
	
}

// Called every frame
void ATrap::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!bIsActivated)
	{
		TrapActivationTimer -= DeltaTime;
		if (TrapActivationTimer<=0)
		{
			SetTrapEnable(true);
		}
	}

}

void ATrap::SetTrapEnable(bool Status)
{
	bIsActivated = Status;
	if (Status)
	{
		GLog->Log("Trap is now activated.");
	}
}

void ATrap::TriggerTrap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If trap is not activated do nothing
	if (!bIsActivated) return;
	
	//AMyCharacter_FirstTry* Caveman = Cast<AMyCharacter_FirstTry>(OtherActor);
	//if (Caveman)
	//{
	//	//TODO:Do dmg to the character
	//	//GLog->Log("Found caveman");
	//	return;
	//}

	AWolfCharacter* Wolf = Cast<AWolfCharacter>(OtherActor);
	if (Wolf)
	{
		//GLog->Log("Trap Found wolf!");
		//Wolf->Stats->CurrentHealth = 0;
		Wolf->Stats->Die();
		Destroy();
		return;
		
	}


}