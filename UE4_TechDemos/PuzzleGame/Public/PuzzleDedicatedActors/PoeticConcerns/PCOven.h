// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/Actor.h"
#include "HelperClasses/MathFunctions.h"
#include "OvenHandle.h"
#include "PCOven.generated.h"


UCLASS()
class PROJECTNA_API APCOven : public AActor
{
	GENERATED_BODY()

private:
	/** Registered oven handles */
	TArray<AOvenHandle*> OvenHandles;

public:
	// Sets default values for this actor's properties
	APCOven();

	/** Registers a new oven handle in the oven.
	* The oven checks every facing direction of the oven handle to meet the win condition
	* @param OvenHandleToRegister - the oven handle instance to register
	*/
	void RegisterOvenHandle(AOvenHandle* OvenHandleToRegister);

	/** Performs a check for the win condition of the puzzle
	* @return true if the puzzle has been solved
	*/
	bool CheckWinCondition();

protected:

	/** The correct direction of each oven handle in order for the puzzle to be solved */
	UPROPERTY(EditAnywhere)
	TArray<EDirection> CorrectDirectionSequence;
};
