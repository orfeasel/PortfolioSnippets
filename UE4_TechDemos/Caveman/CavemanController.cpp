//This is an incomplete source file. It contains limited feautures just for demonstration purposes
void ACavemanController::SetNewTemperature()
{
	float NewTemperature;
	float CurrentTemperature = (Cast<AMyCharacter_FirstTry>(GetPawn()))->Temperature;
	int32 Hours = TimeWidget->Hour;

	//Fixed min and max for new temperature
	float UpperLimitOfNewTemperature = 0;
	float FloorLimitOfNewTemperature = 0;

	//Decide the upper and floor limits
	if ((Hours>=6) && (Hours<=18))
	{
		DetermineNewTemperatureLimits(UpperLimitOfNewTemperature, FloorLimitOfNewTemperature, CurrentTemperature, MaxDayTemperature, MinDayTemperature);
	}
	else
	{
		DetermineNewTemperatureLimits(UpperLimitOfNewTemperature, FloorLimitOfNewTemperature, CurrentTemperature, MaxNightTemperature, MinDayTemperature);
	}

	//Get random based on the upper and floor limits decided above
	NewTemperature = FMath::FRandRange(FloorLimitOfNewTemperature, UpperLimitOfNewTemperature);

	//Set the new temperature
	(Cast<AMyCharacter_FirstTry>(GetPawn()))->Temperature = NewTemperature;
}

void ACavemanController::DetermineNewTemperatureLimits(float& UpperLimit, float& FloorLimit,float CurrentTemperature,float MaxTemp,float MinTemp)
{
	//Determine the new upper limit
	if (CurrentTemperature<=MaxTemp-ClampValue)
	{
		UpperLimit = CurrentTemperature + ClampValue;
	}
	else
	{
		UpperLimit = MaxTemp;
	}
	
	//Determine the lower limit
	if (CurrentTemperature>=MinTemp+ClampValue)
	{
		FloorLimit = CurrentTemperature - ClampValue;
	}
	else
	{
		FloorLimit = MinTemp;
	}
}
