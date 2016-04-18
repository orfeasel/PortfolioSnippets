// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "HealthPickup.h"
#include "RnCCharacter.h"

void AHealthPickup::OnActorOverlaped(AActor* OverlapedActor)
{
	ARnCCharacter* Char = Cast<ARnCCharacter>(OverlapedActor);
	if(Char)
	{
		if (Char->IncreaseHealthBy(HealthToProvide)) Destroy();
	}
}

// Sets default values
AHealthPickup::AHealthPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialization of components

	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));

	SetRootComponent(BoxComp);

	HealthKitSM = CreateDefaultSubobject<UStaticMeshComponent>(FName("HealthKitSM"));

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(FName("ParticleSystemComp"));

	ParticleSystemComp->AttachTo(GetRootComponent());

	HealthKitSM->AttachTo(ParticleSystemComp);

	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>(FName("RotatingMovementComp"));

	//Just an initial value
	HealthToProvide = 10.f;

}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AHealthPickup::OnActorOverlaped);
}


