// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/Actor.h"
#include "MoTPianoKey.h"
#include "Public/MiscActors/ScriptedDoor.h"
#include "MoTPiano.generated.h"


UCLASS()
class PROJECTNA_API AMoTPiano : public AActor
{
	GENERATED_BODY()

private:

	/** True if the puzzle has been solved - false otherwise */
	bool bHasBeenSolved = false;

	/** Contains all the pressed piano keys */
	TArray<AMoTPianoKey*> PressedPianoKeyes;

	/** Determines if the player has entered the correct sequence.
	* When the player plays a wrong tune it empties the PressedPianoKey array
	* @return true if the right tune has been played */
	bool CheckWinCondition();
	
public:	

	// Sets default values for this actor's properties
	AMoTPiano();

	/** Adds a piano key to the entered piano keys array
	* Checks for a win condition after every call
	* @param PressedKey - the piano key */
	void AddPressedPianoKey(AMoTPianoKey* PressedKey);

protected:

	/** The correct piano keys that need to be played */
	UPROPERTY(EditAnywhere, Category = MelodyOfTorment)
	TArray<AMoTPianoKey*> CorrectPianoKeySequence;

	/** Cabin locked door ref - it will open when the puzzle is solved */
	UPROPERTY(EditAnywhere)
	AScriptedDoor* ScriptedDoorRef;
};
