// Orfeas Eleftheriou 2016

#pragma once

#include "Engine/LevelScriptActor.h"
#include "MainMapLevelScript.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API AMainMapLevelScript : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere)
	ABlockingVolume* EntryDockCollider;
	
public:

	virtual void BeginPlay() override;

	void ActivateEntryDockCollider();
	
};
