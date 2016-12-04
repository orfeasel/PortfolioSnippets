// Orfeas Eleftheriou 2016

#pragma once

#include "Items/BaseItem.h"
#include "Components/TimelineHandleComponent.h"
#include "HelperClasses/MathFunctions.h"
#include "OvenHandle.generated.h"


class APCOven;

/**
 * 
 */
UCLASS()
class PROJECTNA_API AOvenHandle : public ABaseItem
{
	GENERATED_BODY()

private:
	/** The function that fires every time the handle is rotating */
	UFUNCTION()
	void TimelineExecutionFunction(FRotator NewRotation);

	/** The function that fires when the handle is finished rotating */
	UFUNCTION()
	void TimelineFinishFunction();

	/** The current facing direction of the handle */
	EDirection FacingDirection;
	
	/** Oven reference */
	APCOven* PCOvenReference;

protected:

	UPROPERTY(VisibleAnywhere)
	UTimelineHandleComponent* TimelineHandleComponent;

	/** The rotator that we want to add to the handle after every interaction */
	UPROPERTY(EditAnywhere)
	FRotator AddedRotator;
	
public:

	AOvenHandle();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual bool Interact(UBackpackComponent& BackpackComponent) override;

	/** Returns the facing direction of the handle
	* @return the current facing direction
	*/
	inline EDirection GetFacingDirection() { return FacingDirection; }

};
