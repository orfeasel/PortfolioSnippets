// Orfeas Eleftheriou 2016

#pragma once

#include "Items/BaseItem.h"
#include "Public/PuzzleDedicatedActors/RoomSlidingPuzzle/RoomCenterNode.h"
#include "RoomTile.generated.h"

/**
 * 
 */

class ARoomTileBoard;

UCLASS()
class PROJECTNA_API ARoomTile : public ABaseItem
{
	GENERATED_BODY()

private:
	/** True if this RoomTile is placed on the RoomTileBoard */
	bool bIsOnTileBoard;

	/** The Current Socket name that this RoomTile occupies when it's placed on the RoomTileBoard */
	FName CurrentSlot;

	/** RoomTileBoard reference */
	ARoomTileBoard* RoomTileBoard = nullptr;

protected:

	/** The associated RoomCenterNode */
	UPROPERTY(EditAnywhere, Category = Puzzle)
	ARoomCenterNode* RoomCenterNode;

	/** True if we want to be able to move this RoomTile when it's placed in the RoomTileBoard */
	UPROPERTY(EditAnywhere, Category = Puzzle)
	bool bShouldMove = true;

public:

	virtual void BeginPlay() override;

	/** Copies the properties of another tile to this RoomTile
	* @param OtherTile - the RoomTile to copy 
	*/
	void PostInit(ARoomTile* OtherTile);

	virtual bool Interact(UBackpackComponent& BackpackComponent) override;

	/** Updates the new status of this RoomTile (if it's placed on the TileBoard or not)
	@ param NewStatus - the new status of the RoomTile
	*/
	inline void SetIsOnTileBoardStatus(bool NewStatus) { bIsOnTileBoard = NewStatus; }

	virtual void OnBeginFocus() override;

	/** Assigns a new socket to this object
	* @param NewCurrentSlot - the new socket that this instance occupies
	*/
	inline void SetCurrentSlot(FName NewCurrentSlot) { CurrentSlot = NewCurrentSlot; }

	/** Returns the current socket name of the RoomTileBoard that this instance occupies
	* @return - the socket name that this object occupies
	*/
	inline FName GetCurrentSlot() { return CurrentSlot; }

	/** Returns the associated RoomCenterNode with this instance
	* @return - the associated RoomCenterNode
	*/
	inline ARoomCenterNode* GetAssociatedRoomCenterNode() { return RoomCenterNode; }

	/** Correlates a new RoomCenterNode to this instance 
	* @param NewNode - the new RoomCenterNode to be associated
	*/
	inline void SetAssociatedRoomCenterNode(ARoomCenterNode* NewNode) { RoomCenterNode = NewNode; }

	/** We use the same, unique tag for every RoomTile object, in order to avoid type casting in every backpack item
	when the player interacts with the RoomTileBoard */
	static const FName RoomTileTag;
	
};
