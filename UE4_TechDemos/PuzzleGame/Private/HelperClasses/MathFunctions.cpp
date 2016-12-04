// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MathFunctions.h"


EDirection MathFunctions::CalculateDirection(FRotator CurrentRotation)
{
	float RollValue = CurrentRotation.Roll;

	float Tolerance = 0.1f;

	if (FMath::IsNearlyEqual(RollValue, 0, Tolerance))
	{
		return EDirection::E_RIGHT;
	}
	else if (FMath::IsNearlyEqual(RollValue, 90, Tolerance))
	{
		return EDirection::E_DOWN;
	}
	else if (FMath::IsNearlyEqual(RollValue, -180, Tolerance))
	{
		return EDirection::E_LEFT;
	}
	else
	{
		return EDirection::E_UP;
	}
}