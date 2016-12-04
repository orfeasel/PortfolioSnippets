// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

//DECLARE_LOG_CATEGORY_EXTERN(BaseItem, Log, All);

class UBackpackComponent;

UCLASS(ABSTRACT)
class PROJECTNA_API ABaseItem : public AActor
{
	GENERATED_BODY()

private:
	/** Plays an interaction sound based on the outcome of the interaction. 
	* If the interaction is successful, the assigned successful sound will get played.
	* If the interaction has failed, the assigned failed sound will get played.
	* @param bIsSuccessful - the outcome of the interaction
	*/
	void PlayInteractionSound(bool bIsSuccessful);

protected:

	/** Prints warnings about potential errors on this instance */
	virtual void PrintWarnings();

	/** If set, the crosshair of the player will change when he sees the current item */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	UTexture2D* CrosshairTexture;

	/** The static mesh of the item */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SM;

	/** The item name -  used when the item is pickupable */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	FString ItemName;

	/** The item name - used when the item is pickupable */
	UPROPERTY(EditAnywhere, Category = Interaction)
	UTexture2D* ItemTexture;

	/** The item that is the player is required to have when he wants to interact with this item */
	UPROPERTY(EditAnywhere, Category = Interaction)
	FString RequiredItemName;

	/** True means that the player will be able to pick up this item */
	UPROPERTY(EditAnywhere, Category = Interaction)
	bool bIsPickupable;

	/** True means that we want to render an outline effect on this item */
	UPROPERTY(EditAnywhere, Category = Interaction)
	bool bShouldRenderOutline=true;

	/** The sound wave of a successful interaction */
	UPROPERTY(EditAnywhere, Category = Interaction)
	USoundBase* SuccessfulSoundCue;

	/** The sound wave of a failed interaction */
	UPROPERTY(EditAnywhere, Category = Interaction)
	USoundBase* FailedSoundCue;
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/** Handles the interaction with this item 
	* @return true if the interaction is successful
	*/
	virtual bool Interact(UBackpackComponent& BackpackComponent);

	/** Is triggered when the player is looking at this item */
	virtual void OnBeginFocus();

	/** Is triggered when the player is no longer looking at this item */
	virtual void OnLostFocus();

	/** Enables or disables the pickup option on this item.
	* @param NewStatus - the new pickup status of the item
	*/
	inline void SetPickupStatus(bool NewStatus) { bIsPickupable = NewStatus; }

	/** Returns the crosshair texture if assigned - nullptr otherwise */
	FORCEINLINE UTexture2D* GetCrosshairTexture() { return CrosshairTexture; }

	/** Returns the item texture */
	FORCEINLINE UTexture2D* GetItemTexture() { return ItemTexture; }

	/** Returns the name of the item */
	FString GetItemName() { return ItemName; }

	/** Returns the name of the item that is neccessary to have in order to interact with this item */
	FString GetRequiredItemName() { return RequiredItemName; }

	inline bool operator==(const ABaseItem& OtherItem)
	{
		return OtherItem.IsValidLowLevelFast() && ItemName.Equals(OtherItem.ItemName);
	}

};
