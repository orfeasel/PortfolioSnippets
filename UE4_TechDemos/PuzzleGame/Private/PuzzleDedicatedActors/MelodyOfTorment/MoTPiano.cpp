// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MoTPiano.h"
#include "MoTPianoKey.h"

bool AMoTPiano::CheckWinCondition()
{
	if (!bHasBeenSolved)
	{
		for (int32 index = 0; index < PressedPianoKeyes.Num(); index++)
		{
			if (PressedPianoKeyes[index] != CorrectPianoKeySequence[index])
			{
				GLog->Log("sound reset on piano");
				PressedPianoKeyes.Empty();
				return false;
			}
		}

		if (PressedPianoKeyes.Num() == CorrectPianoKeySequence.Num())
		{
			
			if (ScriptedDoorRef)
			{
				ScriptedDoorRef->UnlockDoor();
				bHasBeenSolved = true;
			}
		}
	}
	
	return bHasBeenSolved;
}

// Sets default values
AMoTPiano::AMoTPiano()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMoTPiano::AddPressedPianoKey(AMoTPianoKey* PressedKey)
{
	PressedPianoKeyes.Add(PressedKey);

	CheckWinCondition();
}
