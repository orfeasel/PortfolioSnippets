// Orfeas Eleftheriou 2016

#pragma once

UENUM(BlueprintType)
enum class EDirection : uint8
{
	E_UP UMETA(DisplayName="Up"),
	E_DOWN UMETA(DisplayName="Down"),
	E_RIGHT UMETA(DisplayName="Right"),
	E_LEFT UMETA(DisplayName="Left")
};

/**
 * 
 */
static class PROJECTNA_API MathFunctions
{
public:

	/** Calculates the direction that an object is looking based on the Roll value of the provided rotation
	* @param CurrentRotation - the current rotation of the object
	* @return the direction the object is facing
	*/
	static EDirection CalculateDirection(FRotator CurrentRotation);
};
