// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "FootstepComponent.h"


USoundBase* UFootstepComponent::GetSurfaceFootstepSound()
{
	const ACharacter* Character = Cast<ACharacter>(GetOwner());

	FVector StartLocation = Character->GetActorLocation();

	FVector TargetLocation = StartLocation + FVector(0, 0, VerticalRaycastMultiplier) * Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Character);
	QueryParams.bReturnPhysicalMaterial = true;

	FHitResult HitResult;

	const UWorld* World = GetWorld();

	World->LineTraceSingleByChannel(HitResult, StartLocation, TargetLocation, ECC_WorldDynamic, QueryParams);

	if (HitResult.PhysMaterial->IsValidLowLevel())
	{
		uint8 surfaceType = HitResult.PhysMaterial->SurfaceType.GetValue();

		if (WalkableSurfaces.IsValidIndex(surfaceType))
		{
			return WalkableSurfaces[surfaceType];
		}
	}
	return nullptr;
}

// Sets default values for this component's properties
UFootstepComponent::UFootstepComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFootstepComponent::PlayFootstepSound()
{
	if (AudioComponentRef)
	{
		AudioComponentRef->SetSound(GetSurfaceFootstepSound());
		AudioComponentRef->Play();
	}
	else UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetSurfaceFootstepSound(), GetOwner()->GetActorLocation());

}
