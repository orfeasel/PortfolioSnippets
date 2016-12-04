// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "BaseItem.h"
#include "Public/Components/BackpackComponent.h"

DEFINE_LOG_CATEGORY_STATIC(BaseItem, Log, All);

void ABaseItem::PlayInteractionSound(bool bIsSuccessful)
{
	USoundBase* SoundToPlay = (bIsSuccessful) ? SuccessfulSoundCue : FailedSoundCue;

	if (SoundToPlay) UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation());
}

void ABaseItem::PrintWarnings()
{
	if (bIsPickupable && (!ItemTexture || ItemName.IsEmpty()))
	{	
		UE_LOG(BaseItem, Error, TEXT("The %s item is marked as pickupable yet you haven't assigned a texture or a name"), *GetNameSafe(this));
	}

	if (!SuccessfulSoundCue)
	{
		UE_LOG(BaseItem, Warning, TEXT("You haven't assigned a successful sound cue in %s"),*GetNameSafe(this));
	}
}

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SM = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM"));
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();

#if !UE_BUILD_SHIPPING

	PrintWarnings();

#endif
}

bool ABaseItem::Interact(UBackpackComponent& BackpackComponent)
{
	if (!RequiredItemName.IsEmpty() && !bIsPickupable)
	{
		ABaseItem* FoundItem=nullptr;
		
		if (BackpackComponent.GetEquippedItem(FoundItem) && FoundItem->ItemName.Equals(RequiredItemName))
		{
			BackpackComponent.RemoveItemFromBackpack(FoundItem);
			PlayInteractionSound(true);
			return true;
		}
		else
		{
			PlayInteractionSound(false);
			return false;
		}

	}
	else if (bIsPickupable && BackpackComponent.AddItemToBackpack(this))
	{
		PlayInteractionSound(true);
		
		SM->SetVisibility(false, true);
		SM->SetCollisionEnabled(ECollisionEnabled::NoCollision);



		return true;
	}
	else
	{
		PlayInteractionSound(true);
		return true;
	}
}

void ABaseItem::OnBeginFocus()
{
	if(bShouldRenderOutline) SM->SetRenderCustomDepth(true);
}

void ABaseItem::OnLostFocus()
{
	SM->SetRenderCustomDepth(false);
}

