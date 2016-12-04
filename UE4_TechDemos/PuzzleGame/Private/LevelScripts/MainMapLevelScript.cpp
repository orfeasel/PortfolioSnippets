// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MainMapLevelScript.h"





void AMainMapLevelScript::BeginPlay()
{
	Super::BeginPlay();

	if (EntryDockCollider) EntryDockCollider->SetActorEnableCollision(false);
}

void AMainMapLevelScript::ActivateEntryDockCollider()
{
	if (EntryDockCollider) EntryDockCollider->SetActorEnableCollision(true);
}
