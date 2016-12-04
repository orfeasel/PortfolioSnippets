// Orfeas Eleftheriou 2016

#pragma once

#include "Items/BaseItem.h"
#include "Components/TimelineHandleComponent.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API ADoor : public ABaseItem
{
	GENERATED_BODY()
	
private:
	
	/** Rotates the door based on the given value 
	* @param NewRotation - the new rotation
	*/
	UFUNCTION()
	void Rotate(FRotator NewRotation);

	/** Toggles the door open status and resets the collision profile to BlockAllDynamic */
	UFUNCTION()
	void ToggleDoorOpenStatus();

	/** True if the door is closed, false otherwise */
	bool bIsClosed = true;

protected:

	UPROPERTY(VisibleAnywhere)
	UTimelineHandleComponent* TimelineHandleComp;

	/** The yaw value that the door is considered open */
	UPROPERTY(EditAnywhere)
	float MaxOpenYawValue = 90.f;

public:

	ADoor();

	virtual void BeginPlay() override;

	virtual bool Interact(UBackpackComponent& BackpackComponent) override;
	
};
