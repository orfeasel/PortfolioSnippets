// Orfeas Eleftheriou 2016

#pragma once

#include "Items/BaseItem.h"
#include "Items/RoomTile.h"
#include "RoomTileBoard.generated.h"

/**
 * 
 */

struct RoomSocket
{
	bool bIsOccupied;
	FVector WorldLocation;
	FName SocketName;

	RoomSocket(FName SocketName, FVector WorldLocation)
	{
		bIsOccupied = false;
		this->SocketName = SocketName;
		this->WorldLocation = WorldLocation;
	}

	inline bool operator==(const RoomSocket& rhs)
	{
		return this->SocketName.IsEqual(rhs.SocketName);
	}

	inline bool operator!= (const RoomSocket& rhs)
	{
		return !(*this == rhs);
	}

};


UCLASS()
class PROJECTNA_API ARoomTileBoard : public ABaseItem
{
	GENERATED_BODY()

private:

	/** Holds a reference of every socket the SM has */
	TArray<RoomSocket> RoomSockets;

	/** Calculates the nearby sockets based on the given socket name
	* @param CenteredSocket - The center socket of the nearby sockets
	* @return an Array of the nearby sockets
	*/
	TArray<RoomSocket> GetNearbySockets(FName CenteredSocket);

	/** Prints the status of every socket in the output log - Used in debug only */
	void PrintSocketStatus();

	/** Places a tile in the tile board
	* @param OriginalTile - If provided, the newly placed tile copies all of its properties
	*/
	void PlaceTile(ARoomTile* OriginalTile = nullptr);

	/** Holds a reference to the currently moving tile */
	ARoomTile* TileToMove = nullptr;

	/** The initial location of the timeline */
	FVector InitialLocation;

	/** The target location of the timeline */
	FVector TargetLocation;

	/** The timeline that will transform the drawer */
	FTimeline MoveTimeline;

	/** The function that ticks the timeline
	* @param Value - The current value of the associated float curve
	*/
	UFUNCTION()
	void TickTimeline(float Value);


protected:

	/** Holds a reference to the default RoomTile Blueprint class (used in spawning) */
	UPROPERTY(EditAnywhere, Category = Puzzle)
	TSubclassOf<ARoomTile> RoomTileBP;

	/** Holds a reference to the default RoomTile. When the game starts, the referenced tile 
	* is placed automatically in the tileboard
	*/
	UPROPERTY(EditAnywhere, Category = Puzzle)
	ARoomTile* DefaultTileRef;

	/** Contains all the room center actors that are placed in the level */
	UPROPERTY(EditAnywhere, Category = Puzzle)
	TArray<AActor*> RoomCenterActors;

	/** A Float Curve reference used for for animation handling */
	UPROPERTY(EditDefaultsOnly, Category = Puzzle)
	UCurveFloat* FloatCurve;

	/*The total number of rooms. The player can interact
	with a tileboard only when the place tiles equal the TotalRooms number*/
	UPROPERTY(EditDefaultsOnly, Category = Puzzle)
	int32 TotalRooms;

public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual bool Interact(UBackpackComponent& BackpackComponent) override;

	/** Moves a room tile in the tileboard
	* @param TileToMove - the tile to move
	* @return true if the tile has been moved
	*/
	bool MoveTile(ARoomTile* TileToMove);
};
