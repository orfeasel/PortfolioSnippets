// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "WeaponPickup.h"
#include "RnCCharacter.h"
#include "Public/Weapons/WeaponsComponent.h"

//-------------------------------------------------------------------------------------
//Called when something overlaps with the actor
//-------------------------------------------------------------------------------------
void AWeaponPickup::OnActorOverlap(AActor* OverlapedActor)
{
	ARnCCharacter* Char = Cast<ARnCCharacter>(OverlapedActor);

	//If the pickup has a valid weapon - unlock it for the character

	if(Char && WeaponBP)
	{
		UWeaponsComponent* WeaponsComp = Char->GetWeaponsComponent();
		if(WeaponsComp)
		{
			WeaponsComp->UnlockWeapon(WeaponBP->GetDefaultObject<AWeapon>()->GetWeaponType());
			Destroy();
		}
	}
}

// Sets default values
AWeaponPickup::AWeaponPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));

	SetRootComponent(BoxComp);

	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("WeaponSkeletalMesh"));

	WeaponSkeletalMesh->AttachTo(GetRootComponent());

	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>(FName("RotatingMovementComp"));

}

// Called when the game starts or when spawned
void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AWeaponPickup::OnActorOverlap);

}


//-------------------------------------------------------------------------------------
//Updates the skeletal mesh of the weapon pickup if a valid blueprint is assigned
//-------------------------------------------------------------------------------------
void AWeaponPickup::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(WeaponBP)
	{
		WeaponSkeletalMesh->SetSkeletalMesh((WeaponBP->GetDefaultObject<AWeapon>())->GetWeaponSkeletalMesh()->SkeletalMesh);
	}
}
