// Orfeas Eleftheriou 2016

#pragma once

#include "Blueprint/UserWidget.h"
#include "Public/Items/BaseItem.h"
#include "BackpackWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API UBackpackWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	/** Gets all the backpack items
	* @return an array of all the backpack items
	*/
	UFUNCTION(BlueprintCallable, Category = BackpackWidget)
	TArray<ABaseItem*> GetBackpackItems();

	/** The texture reference of the equipped item*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* EquippedItemTexture;
	
public:

	/** Sets a new visibility status for the backpack
	* bNewVisility - the new visibility status (true means we want to render the backpack)
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = BackpackWidget)
	void SetBackpackVisible(bool bNewVisibility);

	/** Updates the texture of the equipped item
	* @param EquippedItem - the current equipped item
	*/
	inline void SetEquippedItemTexture(ABaseItem* EquippedItem)
	{
		EquippedItemTexture = (EquippedItem) ? EquippedItem->GetItemTexture() : nullptr;
	}
};
