// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "DestructibleBox.h"

//-------------------------------------------------------------------------------------
//Calculates the bonus pickup ammo based on given values through the Editor
//-------------------------------------------------------------------------------------
void ADestructibleBox::CalculatePickupAmmo()
{
	AmmoToPickup = FMath::RandRange(MinPickupAmmo, MaxPickupAmmo);
}

//-------------------------------------------------------------------------------------
//Advances the location timeline of the ammo in order to create a "pull" effect
//-------------------------------------------------------------------------------------
void ADestructibleBox::AdvanceLocationTimeline(float Alpha)
{
	if(MainCharacter)
	{
		FVector NewLocation = FMath::Lerp(AmmoSMInitialLocation, MainCharacter->GetActorLocation(), Alpha);
		AmmoSM->SetWorldLocation(NewLocation);

		if((MainCharacter->GetActorLocation()-NewLocation).Size()<=MainCharacter->PickupDistance)
		{
			bShouldStopTimelines = true;

			LocationTimeline.Stop();
			RotationTimeline.Stop();

			//If the destructible doesn't contain a valid bp reference check if a random reference exists
			if(RangeWeaponBP)
			{
				MainCharacter->IncreaseAmmo(AmmoToPickup, RangeWeaponBP->GetDefaultObject<ARangeWeapon>()->GetWeaponType());
			}
			else if (RangeWeapon) MainCharacter->IncreaseAmmo(AmmoToPickup, RangeWeapon->GetWeaponType());
			

			AmmoSM->SetHiddenInGame(true);


			//Destroy the actor after the specified time
			FTimerHandle TimerHandler;
			FTimerDelegate TimerDelegate;

			TimerDelegate.BindLambda([&]()
			{
				Destroy();
			});

			GetWorld()->GetTimerManager().SetTimer(TimerHandler, TimerDelegate, DestructionTime, false);
			
		}

	}
}

//-------------------------------------------------------------------------------------
//Rotates the ammo sm
//-------------------------------------------------------------------------------------
void ADestructibleBox::AdvanceRotationTimeline(float Alpha)
{
	if(MainCharacter)
	{
		FRotator NewRotation = FMath::Lerp(GetActorRotation(), EndRotation, Alpha);
		AmmoSM->SetWorldRotation(NewRotation);
	}
}

// Sets default values
ADestructibleBox::ADestructibleBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize the necessary components
	DesctructibleSM = CreateDefaultSubobject<UDestructibleComponent>(FName("DestructibleSM"));

	SetRootComponent(DesctructibleSM);

	AmmoSM = CreateDefaultSubobject<UStaticMeshComponent>(FName("AmmoSM"));

	//Determine the static mesh of the pickup based on the ammo type that this box provides
	if(RangeWeaponBP)
	{
		UStaticMesh* PickupSM = RangeWeaponBP->GetDefaultObject<ARangeWeapon>()->GetAmmoPickupSM();

		AmmoSM->StaticMesh = PickupSM;
	}

	//Initialize properties
	AmmoSM->SetRelativeLocation(FVector::ZeroVector);

	bHasBeenDestroyed = false;
	bShouldStopTimelines = false;

	DestructionTime = 1.f;
}

// Called when the game starts or when spawned
void ADestructibleBox::BeginPlay()
{
	Super::BeginPlay();

	//Calculate the pickup ammo
	CalculatePickupAmmo();
	
	//Initialize the timelines for location and rotation lerp
	if(LocationCurve && RotationCurve && AmmoSM)
	{
		FOnTimelineFloat LocationProgressFunction;
		FOnTimelineFloat RotationProgressFunction;

		LocationProgressFunction.BindUFunction(this, FName("AdvanceLocationTimeline"));
		RotationProgressFunction.BindUFunction(this, FName("AdvanceRotationTimeline"));

		LocationTimeline.AddInterpFloat(LocationCurve, LocationProgressFunction);
		RotationTimeline.AddInterpFloat(RotationCurve, RotationProgressFunction);


		AmmoSMInitialLocation = GetActorLocation();
	}

	MainCharacter = Cast<ARnCCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//If the box doesn't contain an associated range weapon bp it should decide
	//a random type of ammo 
	if(!RangeWeaponBP)
	{
		if(MainCharacter)
		{
			ARangeWeapon* RandomRangeWeapon = Cast<ARangeWeapon>(MainCharacter->GetWeaponsComponent()->GetRandomWeaponReference(false));
			
			if(RandomRangeWeapon)
			{
				RangeWeapon = RandomRangeWeapon;

				UStaticMesh* PickupSM = RandomRangeWeapon->GetAmmoPickupSM();

				AmmoSM->StaticMesh = PickupSM;
			}
			else Destroy();
		}
	}
}

// Called every frame
void ADestructibleBox::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if(bHasBeenDestroyed && !bShouldStopTimelines)
	{
		//The box has been destroyed - tick the timelines
		LocationTimeline.TickTimeline(DeltaTime);
		RotationTimeline.TickTimeline(DeltaTime);
	}

}

//-------------------------------------------------------------------------------------
//This implementation of TakeDamage is "fixed" in order for the box to break with 1 hit
//-------------------------------------------------------------------------------------
float ADestructibleBox::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DesctructibleSM->ApplyRadiusDamage(DamageAmount + BaseDamage, GetActorLocation(), DamageRadius, ImpulseStrength, true);
	
	//The destructible has been destroyed - start the timelines
	bHasBeenDestroyed = true;

	LocationTimeline.PlayFromStart();
	RotationTimeline.PlayFromStart();

	return BaseDamage;
}

//-------------------------------------------------------------------------------------
//Updates the static mesh of the ammo from within the editor
//-------------------------------------------------------------------------------------
void ADestructibleBox::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//Determine the static mesh of the pickup based on the ammo type that this box provides
	if (RangeWeaponBP)
	{
		UStaticMesh* PickupSM = RangeWeaponBP->GetDefaultObject<ARangeWeapon>()->GetAmmoPickupSM();

		AmmoSM->StaticMesh = PickupSM;
	}
	else AmmoSM->StaticMesh = nullptr;

	AmmoSM->SetRelativeLocation(FVector::ZeroVector);


}
