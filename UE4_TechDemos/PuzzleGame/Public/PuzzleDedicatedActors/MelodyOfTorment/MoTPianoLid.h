// Orfeas Eleftheriou 2016

#pragma once

#include "Items/BaseItem.h"
#include "Components/TimelineHandleComponent.h"
#include "MoTPianoLid.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API AMoTPianoLid : public ABaseItem
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void Rotate(FRotator NewRotation);

	bool bIsOpen = false;
	
protected:
	UPROPERTY(VisibleAnywhere)
	UTimelineHandleComponent* TimelineHandleComponent;

	AMoTPianoLid();

public:

	virtual void BeginPlay() override;

	virtual bool Interact(UBackpackComponent& BackpackComponent) override;
	
};
