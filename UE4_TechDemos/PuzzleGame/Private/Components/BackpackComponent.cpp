// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "BackpackComponent.h"
#include "Public/Controllers/MainCharacterController.h"
#include "Public/Character/MainCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(BackpackComponent, All, All);

// Sets default values for this component's properties
UBackpackComponent::UBackpackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBackpackComponent::BeginPlay()
{
	Super::BeginPlay();

	BackpackItems.SetNum(MaxBackpackItems);
}


// Called every frame
void UBackpackComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

bool UBackpackComponent::AddItemToBackpack(ABaseItem* ItemToAdd)
{
	const AMainCharacter* MainCharacter = Cast<AMainCharacter>(GetOwner());

	for (int32 i = 0; i < BackpackItems.Num(); i++)
	{
		if (!BackpackItems[i])
		{
			BackpackItems[i] = ItemToAdd;

			MainCharacter->GetActionsTutorialComponent()->DisplayTutorialMessage(ETutorialMessageType::EPickupTutorial);
			return true;
		}
	}

	return false;
}

void UBackpackComponent::RemoveItemFromBackpack(ABaseItem* ItemToRemove)
{
	EquipItem(nullptr);

	for (int32 i = 0; i < BackpackItems.Num(); i++)
	{
		if (BackpackItems[i] && BackpackItems[i] == ItemToRemove)
		{
			BackpackItems[i] = nullptr;
		}
	}
}

void UBackpackComponent::RemoveItemFromBackpack(FString NameOfItemToRemove)
{
	EquipItem(nullptr);

	for (int32 i = 0; i < BackpackItems.Num();i++)
	{
		if (BackpackItems[i] && BackpackItems[i]->GetItemName().Equals(NameOfItemToRemove))
		{
			BackpackItems[i] = nullptr;
		}
	}
}

bool UBackpackComponent::ContainsItem(FString NameOfItem, ABaseItem*& FoundItem)
{
	if (!NameOfItem.IsEmpty())
	{
		for (int32 i = 0; i < BackpackItems.Num();i++)
		{
			if (BackpackItems[i] && BackpackItems[i]->GetItemName().Equals(NameOfItem))
			{
				FoundItem = BackpackItems[i];
				return true;
			}
		}
		return false;
	}
	return true;
}

void UBackpackComponent::EquipItem(UTexture2D* TextureOfItem)
{
	if (TextureOfItem)
	{
		EquippedItem = *BackpackItems.FindByPredicate([&TextureOfItem](ABaseItem* CurrentItem)
		{
			return (CurrentItem && CurrentItem->GetItemTexture() == TextureOfItem) ? CurrentItem : nullptr;
		});

	}
	else
	{
		EquippedItem = nullptr;
	}
	Cast<AMainCharacterController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))->SetEquippedItem(EquippedItem);
}

bool UBackpackComponent::GetEquippedItem(ABaseItem*& CurrentlyEquippedItem)
{
	CurrentlyEquippedItem = this->EquippedItem;

	return (CurrentlyEquippedItem) ? true : false;
}

void UBackpackComponent::RemoveEquippedItem()
{
	EquipItem(nullptr);
}

