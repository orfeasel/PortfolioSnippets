// Fill out your copyright notice in the Description page of Project Settings.

#include "RoguelikeSurvival.h"
#include "Pickupable.h"


// Sets default values
APickupable::APickupable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Static mesh initial settings
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->BodyInstance.SetObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMesh->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//Particle emmiter initialization
	ParticleEmitter = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PrimaryParticleEmitter"));
	ParticleEmitter->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	//Audio component initialization
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->bAutoActivate = false;

	//Attaching
	SetRootComponent(StaticMesh);
	ParticleEmitter->AttachTo(GetRootComponent());
	//StaticMesh->AttachTo(GetRootComponent());
	AudioComp->AttachTo(GetRootComponent());

	//Setting up rotating movement component
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
	FRotator RotationRate = FRotator(0, 180, 0);
	RotatingMovementComponent->RotationRate = RotationRate;

	SecondsToDestroy = 1.5f;
}

// Called when the game starts or when spawned
void APickupable::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APickupable::ActorOverlaped);
	
}

// Called every frame
void APickupable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void APickupable::ActorOverlaped(AActor* Other)
{
	GLog->Log("Overlaped");
}

void APickupable::Kill()
{
	//Disables the collision and the visibility of the actor
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMesh->SetVisibility(false, true);

	//Marks the actor as pending kill
	this->MarkPendingKill();
	
	FTimerHandle TimerHandle;

	FTimerDelegate Del;
	Del.CreateLambda([&]() {this->Destroy(); });

	//Destroys the actor after specified delay
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Del, SecondsToDestroy ,false);
}
