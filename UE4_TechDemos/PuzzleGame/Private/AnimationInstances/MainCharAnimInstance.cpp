// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MainCharAnimInstance.h"
#include "Public/Character/MainCharacter.h"
#include "Public/Components/FootstepComponent.h"


void UMainCharAnimInstance::PlayFootstepSound()
{
	const AMainCharacter* MainChar = Cast<AMainCharacter>(TryGetPawnOwner());
	MainChar->GetFootstepComponent()->PlayFootstepSound();
}

void UMainCharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const APawn* MainCharacterPawn = TryGetPawnOwner();

	if (MainCharacterPawn)
	{
		const UPawnMovementComponent* PawnMovementComponent = MainCharacterPawn->GetMovementComponent();

		bIsInAir = PawnMovementComponent->IsFalling();

		//The reason we're checking if the movement component is active
		//is because sometimes the StopMovementImmediately doesn't set the velocity of the pawn to zero in time
		//This may result in opening the inventory and still listen to movement effects even though we're not moving

		bIsMoving = (PawnMovementComponent->IsActive() && PawnMovementComponent->Velocity.Size() > 0) ? true : false;
	}
}
