// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "PCOven.h"


// Sets default values
APCOven::APCOven()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void APCOven::RegisterOvenHandle(AOvenHandle* OvenHandleToRegister)
{
	if (OvenHandleToRegister)
	{
		OvenHandles.Add(OvenHandleToRegister);
	}
}

bool APCOven::CheckWinCondition()
{
	for (int32 index = 0; index < OvenHandles.Num(); index++)
	{
		if (OvenHandles[index]->GetFacingDirection() != CorrectDirectionSequence[index])
		{
			GLog->Log("mpales");
			return false;
		}
	}
	GLog->Log("Poetic concerns has been solved");
	return true;
}
