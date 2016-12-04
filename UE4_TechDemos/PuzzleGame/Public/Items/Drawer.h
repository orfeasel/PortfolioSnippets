// Orfeas Eleftheriou 2016

#pragma once

#include "Items/BaseItem.h"
#include "Public/Components/TimelineHandleComponent.h"
#include "Drawer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API ADrawer : public ABaseItem
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void TimelineExecutionFunction(FVector NewLocation);

public:

	ADrawer();

	virtual void BeginPlay() override;

	virtual bool Interact(UBackpackComponent& BackpackComponent) override;

	/** Prints warnings about potential errors on this instance */
	virtual void PrintWarnings() override;

protected:

	/*The distance that the drawer will cover when the player opens it*/
	UPROPERTY(EditAnywhere)
	float DrawLength = 50.f;

	UPROPERTY(VisibleAnywhere)
	UTimelineHandleComponent* TimelineHandleComp;
	
};
