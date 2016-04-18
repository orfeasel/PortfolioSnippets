// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "JumpNode.h"
#include "RnCCharacter.h"


void AJumpNode::ActorBeginOverlap(AActor* OverlapedActor)
{
	ARnCCharacter* Char = Cast<ARnCCharacter>(OverlapedActor);
	if (Char)
	{
		Char->SetJumpNode(this);
	}
}

void AJumpNode::ActorEndOverlap(AActor* OtherActor)
{
	ARnCCharacter* Char = Cast<ARnCCharacter>(OtherActor);
	if (Char)
	{
		Char->SetJumpNode(nullptr);
	}
}

//-------------------------------------------------------------------------------------
//Advances the timeline by delta time
//-------------------------------------------------------------------------------------
void AJumpNode::AdvanceTimeline(float DeltaTime)
{
	FVector NewLocation = FMath::Lerp(StartingLocationOfInterpolationActor, JumpNodeSM->GetComponentLocation(), DeltaTime);
	InterpolatingActor->SetActorLocation(NewLocation);
}

// Sets default values
AJumpNode::AJumpNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));

	SetRootComponent(BoxComp);

	JumpNodeSM = CreateDefaultSubobject<UStaticMeshComponent>(FName("JumpNodeSM"));

	JumpNodeSM->AttachTo(BoxComp);

	InterpolatingActor = nullptr;

}

// Called when the game starts or when spawned
void AJumpNode::BeginPlay()
{
	Super::BeginPlay();

	//Register overlaps
	OnActorBeginOverlap.AddDynamic(this, &AJumpNode::ActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &AJumpNode::ActorEndOverlap);
	

	if(InterpolationCurve)
	{
		FOnTimelineFloat InterpolationProgressFunction;

		InterpolationProgressFunction.BindUFunction(this, FName("AdvanceTimeline"));

		InterpTimeline.AddInterpFloat(InterpolationCurve, InterpolationProgressFunction);
	}
}

void AJumpNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(InterpolatingActor)
	{
		//If the actor we're interpolating is within break distance stop the timeline and break the reference
		//to that actor
		if((InterpolatingActor->GetActorLocation()-JumpNodeSM->GetComponentLocation()).Size()<=InterpolationBreakDistance)
		{
			InterpTimeline.Stop();
			InterpolatingActor = nullptr;

			//Deactivate the particle system
			if (ParticleComp) ParticleComp->DeactivateSystem();
		}
		else
		{
			//The actor we're interpolating is in valid distance - continue the "pull" effect
			InterpTimeline.TickTimeline(DeltaTime);
		}
	}

}

//-------------------------------------------------------------------------------------
//Interpolate the location of the main character. You can extend this to apply to other
//actors too.
//-------------------------------------------------------------------------------------
void AJumpNode::ApplyLerp(AActor* ActorToInterpolate)
{
	if(ActorToInterpolate)
	{
		//Assign a reference to the actor, get it's initial location and start the interpolation
		InterpolatingActor = ActorToInterpolate;
		
		ARnCCharacter* MainCharacter = Cast<ARnCCharacter>(InterpolatingActor);

		//The character can only get attached to the jump node when he has a range weapon currently equipped
		if(MainCharacter && !MainCharacter->IsMeleeWeaponEquipped())
		{
			ARangeWeapon* RangeWeapon = Cast<ARangeWeapon>(MainCharacter->GetCurrentlyEquippedWeapon());

			if(RangeWeapon)
			{
				//Spawn the particle system and attach it to the muzzle of the character's gun and the static mesh of the jump node

				ParticleComp = UGameplayStatics::SpawnEmitterAttached(ArcParticle, RangeWeapon->GetRootComponent(), RangeWeapon->GetMuzzleSocketName(), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale);

				ParticleComp->SetBeamTargetPoint(0, JumpNodeSM->GetComponentLocation(), 0);

				StartingLocationOfInterpolationActor = InterpolatingActor->GetActorLocation();

				InterpTimeline.PlayFromStart();
			}

		}
	}
}

