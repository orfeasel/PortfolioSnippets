// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "ButtlerAnimInstance.h"
#include "Character/ButtlerCharacter.h"




void UButtlerAnimInstance::PlayFootstepSound()
{
	const AButtlerCharacter* Buttler = Cast<AButtlerCharacter>(TryGetPawnOwner());

	if (Buttler) Buttler->GetFootstepComponent()->PlayFootstepSound();
}

void UButtlerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn) MovementSpeed = Pawn->GetMovementComponent()->Velocity.Size();
}
