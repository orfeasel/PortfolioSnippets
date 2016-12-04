// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "BackpackItemWidget.h"
#include "Public/Character/MainCharacter.h"
#include "Public/Components/BackpackComponent.h"

void UBackpackItemWidget::UpdateEquippedItem()
{
	if (ItemTexture)
	{
		UBackpackComponent* CharacterBackpack = (Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))->GetBackpackComponent();

		CharacterBackpack->EquipItem(ItemTexture);
	}
}

void UBackpackItemWidget::SetItemTexture(ABaseItem* Item)
{
	ItemTexture = (Item) ? Item->GetItemTexture() : nullptr;
}

