// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MoTPianoKey.h"
#include "MoTPiano.h"


bool AMoTPianoKey::Interact(UBackpackComponent& BackpackComponent)
{
	Super::Interact(BackpackComponent);

	AMoTPiano* MoTPiano = Cast<AMoTPiano>(GetAttachParentActor());

	MoTPiano->AddPressedPianoKey(this);

	return true;
}
