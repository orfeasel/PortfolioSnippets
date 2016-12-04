// Orfeas Eleftheriou 2016

#pragma once

#include "Components/ActorComponent.h"
#include "Public/Items/BaseItem.h"
#include "BackpackComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNA_API UBackpackComponent : public UActorComponent
{
	GENERATED_BODY()


private:

	/** The items of the backpack */
	UPROPERTY(VisibleAnywhere)
	TArray<ABaseItem*> BackpackItems;

	/** The equipped item reference */
	ABaseItem* EquippedItem = nullptr;

public:	
	// Sets default values for this component's properties
	UBackpackComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/** Adds the given item to the backpack.
	* @param ItemToAdd - The Item to add to the backpack
	* @return true if the item is added, false otherwise
	*/
	bool AddItemToBackpack(ABaseItem* ItemToAdd);

	/** Removes an item from the backpack
	* @param ItemToRemove - The item to remove from the backpack
	*/
	void RemoveItemFromBackpack(ABaseItem* ItemToRemove);

	/** Removes an item from the backpack
	* @param NameOfItemToRemove - The name of the item to remove.
	*/
	void RemoveItemFromBackpack(FString NameOfItemToRemove);

	/** Checks if an item exists inside the backpack
	* @param NameOfItem - The item's name to search
	* @param FoundItem - A reference of the found item
	* @return true if the item is found, false otherwise
	*/
	bool ContainsItem(FString NameOfItem, ABaseItem*& FoundItem);

	/** Returns a collection of all the item of the backpack
	*@return All the items of the backpack 
	*/
	inline TArray<ABaseItem*> GetItems() const { return BackpackItems; }

	/*UFUNCTION(BlueprintCallable, Category = Backpack)
	void EquipItem(int32 IndexOfItem);*/

	/** Searches all the backpack items until it finds an item with a matching texture.
	* If the item is found, the player equips it and the UI gets updated
	* @param TextureOfItem - the texture of the item we want to equip
	*/
	void EquipItem(UTexture2D* TextureOfItem);

	/** Gets the item that is currently equipped
	* @param ABaseItem - the equipped item reference
	* @return true if the player is currently equipped with an item
	*/
	bool GetEquippedItem(ABaseItem*& CurrentlyEquippedItem);

	/** Removes the currently equipped item */
	void RemoveEquippedItem();

protected:

	/** The max numbers of backpack items*/
	UPROPERTY(EditDefaultsOnly)
	int32 MaxBackpackItems;

};
