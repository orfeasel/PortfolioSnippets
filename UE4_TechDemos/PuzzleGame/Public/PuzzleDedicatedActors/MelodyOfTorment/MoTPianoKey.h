// Orfeas Eleftheriou 2016

#pragma once

#include "Items/BaseItem.h"
#include "MoTPianoKey.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API AMoTPianoKey : public ABaseItem
{
	GENERATED_BODY()

public:

	virtual bool Interact(UBackpackComponent& BackpackComponent) override;
};
