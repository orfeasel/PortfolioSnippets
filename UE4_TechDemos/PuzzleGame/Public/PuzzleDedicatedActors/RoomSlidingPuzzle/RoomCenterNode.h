// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/Actor.h"
#include "RoomCenterNode.generated.h"

UCLASS()
class PROJECTNA_API ARoomCenterNode : public AActor
{
	GENERATED_BODY()

private:
	/** True if the room has appeared in the map */
	bool bHasBeenPlacedInLevel = false;

	/** The StartLocation of the animation */
	FVector StartLocation;

	/** The TargetLocation of the animation */
	FVector TargetLocation;

public:	

	/** Places the room in a new location and makes all the attached actors visible
	* @param NewLocation - the new location of the room
	*/
	void PlaceInLevel(const FVector& NewLocation);

	/** Returns the start location of the animation in world coordinates
	* @return - the start location of the animation
	*/
	inline FVector GetStartLocation() { return StartLocation; }

	/** Returns the target location of the animation in world coordinates
	* @return - the target location of the animation
	*/
	inline FVector GetFinishLocation() { return TargetLocation; }

	/** Calculates the start location for the next animation based on the Actor's world location */
	UFUNCTION()
	void ReCalculateStartLocation() { StartLocation = GetActorLocation(); }

	/** Assigns a new target location for the animation */
	void SetTargetLocation(FVector NewTargetLocation) { TargetLocation = NewTargetLocation; }
};
