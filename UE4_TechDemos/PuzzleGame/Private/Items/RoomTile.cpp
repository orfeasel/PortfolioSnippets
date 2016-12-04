// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "RoomTile.h"
#include "Public/Components/BackpackComponent.h"
#include "Items/RoomTileBoard.h"

const FName ARoomTile::RoomTileTag = "RoomTile";

void ARoomTile::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(RoomTileTag);
}

void ARoomTile::PostInit(ARoomTile* OtherTile)
{
	if (!OtherTile) return;

	SM->SetMaterial(0, OtherTile->SM->GetMaterial(0));
	RoomCenterNode = OtherTile->RoomCenterNode;
	bShouldMove = OtherTile->bShouldMove;
}

bool ARoomTile::Interact(UBackpackComponent& BackpackComponent)
{
	if (!bIsOnTileBoard)
	{
		Super::Interact(BackpackComponent);

		bIsOnTileBoard = true;

		return true;
	}
	else if(bShouldMove)
	{
		if(!RoomTileBoard) RoomTileBoard = Cast<ARoomTileBoard>(GetOwner());

		USoundBase* SoundToPlay = (RoomTileBoard->MoveTile(this)) ? SuccessfulSoundCue : FailedSoundCue ;

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation());

		return true;
	}

	return false;
}

void ARoomTile::OnBeginFocus()
{
	if (!bIsOnTileBoard) SM->SetRenderCustomDepth(true);
}
