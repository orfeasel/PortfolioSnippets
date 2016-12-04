// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "Document.h"
#include "Public/Controllers/MainCharacterController.h"
#include "Public/Character/MainCharacter.h"
#include "Public/Components/ActionsTutorialComponent.h"


bool ADocument::Interact(UBackpackComponent& BackpackComponent)
{
	Super::Interact(BackpackComponent);

	AMainCharacterController* Controller=Cast<AMainCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	AMainCharacter* MainChar = Cast<AMainCharacter>(Controller->GetCharacter());

	MainChar->GetActionsTutorialComponent()->EraseDisplayingMessage();

	Controller->ReadDocument(this);

	return true;
}
