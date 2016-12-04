// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "BackpackWidget.h"
#include "Public/Character/MainCharacter.h"
#include "Public/Components/BackpackComponent.h"



TArray<ABaseItem*> UBackpackWidget::GetBackpackItems()
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	return MainCharacter->GetBackpackComponent()->GetItems();
}
