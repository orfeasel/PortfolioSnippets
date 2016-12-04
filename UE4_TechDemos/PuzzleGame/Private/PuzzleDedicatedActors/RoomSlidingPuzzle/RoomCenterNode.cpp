// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "RoomCenterNode.h"


void ARoomCenterNode::PlaceInLevel(const FVector& NewLocation)
{
	if (!bHasBeenPlacedInLevel)
	{
		TArray<AActor*> AttachedActors;
		
		GetAttachedActors(AttachedActors);
		for (auto It = AttachedActors.CreateIterator(); It; It++)
		{
			(*It)->SetActorHiddenInGame(false);

			//Initially, all hidden static meshes have a disabled collision through the editor,
			//we need to change their profile name in order to enable the collision we desire

			if ((*It)->IsA<AStaticMeshActor>())
			{
				AStaticMeshActor* SM = Cast<AStaticMeshActor>(*It);
				SM->GetStaticMeshComponent()->SetCollisionProfileName(FName("BlockAll"));
			}
		}

		bHasBeenPlacedInLevel = true;
	}

	if(SetActorLocation(NewLocation)) StartLocation = NewLocation;
}

