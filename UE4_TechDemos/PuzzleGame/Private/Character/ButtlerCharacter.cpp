// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "ButtlerCharacter.h"
#include "Public/Controllers/ButtlerController.h"

// Sets default values
AButtlerCharacter::AButtlerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FootstepComp = CreateDefaultSubobject<UFootstepComponent>(FName("FootstepComp"));

	FootstepAudioComp = CreateDefaultSubobject<UAudioComponent>(FName("FootstepAudioComp"));
	FootstepAudioComp->SetupAttachment(GetRootComponent());

	FootstepComp->SetAttachedAudioComponent(FootstepAudioComp);
}

void AButtlerCharacter::WalkToCabin()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), WelcomeSFX, GetActorLocation());

	AButtlerController* ButtlerCon = Cast<AButtlerController>(GetController());

	ButtlerCon->SetMoveLocation(ActorToMoveAt);

	ButtlerCon->SetActorToFace(ActorToFace);
}

void AButtlerCharacter::InitializeExit()
{
	AButtlerController* ButtlerCon = Cast<AButtlerController>(GetController());

	ButtlerCon->SetExitActor(ExitActor);
}

