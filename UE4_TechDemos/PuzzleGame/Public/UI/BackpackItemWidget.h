// Orfeas Eleftheriou 2016

#pragma once

#include "Blueprint/UserWidget.h"
#include "Public/Items/BaseItem.h"
#include "BackpackItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API UBackpackItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	/** The item's texture reference */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* ItemTexture;

	/** Updates the equipped item (called via button click on BP graph) */
	UFUNCTION(BlueprintCallable, Category = BackpackWidget)
	void UpdateEquippedItem();

public:

	/** Sets a new texture based on the given item
	* @param Item - the item that this widget represents
	*/
	UFUNCTION(BlueprintCallable, Category = BackpackWidget)
	void SetItemTexture(ABaseItem* Item);
	
	
};
