// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "RoomTileBoard.h"
#include "Public/Components/BackpackComponent.h"

/*
Visual representation of the tileboard
X means that a tile exists in that location
# means that we have an available spot in that location
The tile board is 3 x 2
 _ _ _ _ 
|X # X |
|X X _ |
|_ _ _ |


The nearby sockets of each tile, are given by the
the spot of the socket +- 1 and +- 3. This is described in the
GetNearbySockets function which is located below.
*/

TArray<RoomSocket> ARoomTileBoard::GetNearbySockets(FName CenteredSocket)
{
	TArray<RoomSocket> NearbySockets;

	for (int32 i = 0; i < RoomSockets.Num(); i++)
	{
		if (RoomSockets[i].SocketName.IsEqual(CenteredSocket))
		{
			if (RoomSockets.IsValidIndex(i - 1)) NearbySockets.Add(RoomSockets[i - 1]);

			if (RoomSockets.IsValidIndex(i + 1)) NearbySockets.Add(RoomSockets[i + 1]);

			if (RoomSockets.IsValidIndex(i + 3)) NearbySockets.Add(RoomSockets[i + 3]);

			if (RoomSockets.IsValidIndex(i - 3)) NearbySockets.Add(RoomSockets[i - 3]);

			return NearbySockets;
		}
	}

	return NearbySockets;
}

void ARoomTileBoard::PrintSocketStatus()
{
	for (auto RoomSocket : RoomSockets)
	{
		GLog->Log(RoomSocket.SocketName.ToString() + " - " + (RoomSocket.bIsOccupied ? FString("True") : FString("False")));
	}

	GLog->Log("-------------------------------------------");
}

void ARoomTileBoard::PlaceTile(ARoomTile* OriginalTile)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	//Find the first unoccupied socket index
	int32 AvailableSocketIndex = RoomSockets.IndexOfByPredicate([&](RoomSocket s)
	{
		return !s.bIsOccupied;
	});

	//Spawn a tile in the corresponding location
	ARoomTile* TileToSpawn = GetWorld()->SpawnActor<ARoomTile>(RoomTileBP, RoomSockets[AvailableSocketIndex].WorldLocation, FRotator::ZeroRotator, SpawnParams);

	//If we have a valid reference to the original tile, copy its properties to
	//our newly placed tile
	if (OriginalTile)
	{
		TileToSpawn->PostInit(OriginalTile);
		OriginalTile->GetAssociatedRoomCenterNode()->PlaceInLevel(RoomCenterActors[AvailableSocketIndex]->GetActorLocation());
	}

	//Store the current slot of the tile and set its tileboard placement status to true
	TileToSpawn->SetCurrentSlot(RoomSockets[AvailableSocketIndex].SocketName);
	TileToSpawn->SetIsOnTileBoardStatus(true);

	//Mark the corresponding socket as occupied in order to avoid placing a new tile in the same spot
	RoomSockets[AvailableSocketIndex].bIsOccupied = true;
}

void ARoomTileBoard::TickTimeline(float Value)
{
	//Since we want to move the tiles with the same speed as the whole rooms,
	//we use the same curve to determine the new location for both the room tiles
	//and the room center nodes.

	//Perform a linear interpolation to determine the next location of the tile
	FVector NewTileLocation = FMath::Lerp(InitialLocation, TargetLocation, Value);
	TileToMove->SetActorLocation(NewTileLocation);

	ARoomCenterNode* RoomCenterNode = TileToMove->GetAssociatedRoomCenterNode();

	//Perform a linear interpolation to determine the next location of the room center node
	FVector NewRoomLocation = FMath::Lerp(RoomCenterNode->GetStartLocation(), RoomCenterNode->GetFinishLocation(), Value);
	RoomCenterNode->SetActorLocation(NewRoomLocation);
}

void ARoomTileBoard::BeginPlay()
{
	Super::BeginPlay();


	//Store all the available sockets
	TArray<FName> Sockets = SM->GetAllSocketNames();

	for (auto It:Sockets)
	{
		RoomSockets.Add(RoomSocket(It,SM->GetSocketLocation(It)));
	}

	if (FloatCurve)
	{
		//Create a progress function to handle the assigned float curve
		//and bind it in the assigned timeline
		FOnTimelineFloat ProgressFunction;

		ProgressFunction.BindUFunction(this, FName("TickTimeline"));

		MoveTimeline.AddInterpFloat(FloatCurve, ProgressFunction);
	}

	//Place the default tile
	PlaceTile(DefaultTileRef);
}

void ARoomTileBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MoveTimeline.IsPlaying())
	{
		MoveTimeline.TickTimeline(DeltaTime);
	}
}

bool ARoomTileBoard::Interact(UBackpackComponent& BackpackComponent)
{
	const TArray<ABaseItem*> Items = BackpackComponent.GetItems();

	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i] && Items[i]->ActorHasTag(ARoomTile::RoomTileTag))
		{
			BackpackComponent.RemoveItemFromBackpack(Items[i]);

			//Place the given tile
			PlaceTile(Cast<ARoomTile>(Items[i]));

			return true;

			//PrintSocketStatus();
		}
	}
	return false;
}

bool ARoomTileBoard::MoveTile(ARoomTile* TileToMove)
{
	if (TileToMove && !MoveTimeline.IsPlaying() && Children.Num()==TotalRooms)
	{
		TArray<RoomSocket> NearbySockets = GetNearbySockets(TileToMove->GetCurrentSlot());

		for (int32 i = 0; i < NearbySockets.Num(); i++)
		{
			//If the nearby socket is marked as unoccupied, we have a new valid location
			//for our room tile and our room
			if (!NearbySockets[i].bIsOccupied)
			{
				for (int32 j = 0; j < RoomSockets.Num(); j++)
				{
					//Mark the previous room socket as unoccupied, since we are about to move the room tile
					if (RoomSockets[j].SocketName.IsEqual(TileToMove->GetCurrentSlot()))
					{
						RoomSockets[j].bIsOccupied = false;
					}

					//Mark the new room socket as occupied and set a new target location for our room center node
					if (RoomSockets[j] == NearbySockets[i])
					{
						RoomSockets[j].bIsOccupied = true;
						TileToMove->GetAssociatedRoomCenterNode()->SetTargetLocation(RoomCenterActors[j]->GetActorLocation());
					}
				}

				//Store the initial and target locations for our tile movement
				InitialLocation = TileToMove->GetActorLocation();
				TargetLocation = NearbySockets[i].WorldLocation;

				this->TileToMove = TileToMove;

				//Update the current slot of our tile
				TileToMove->SetCurrentSlot(NearbySockets[i].SocketName);

				//Assign a function ( must be marked as UFUNCTION() ) to be fired when the timeline finishes
				FOnTimelineEventStatic TimelineEvent;
				TimelineEvent.BindUFunction(TileToMove->GetAssociatedRoomCenterNode(), FName("ReCalculateStartLocation"));
				MoveTimeline.SetTimelineFinishedFunc(TimelineEvent);

				//Start the movement of our room tile and room center node
				MoveTimeline.PlayFromStart();

				return true;
			}
		}
	}
	return false;
}
