// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "CharacterRaycastComponent.h"
#include "Public/Items/BaseItem.h"
#include "Public/Components/ActionsTutorialComponent.h"

void UCharacterRaycastComponent::SearchForUsableItems()
{
	if (SeeingActor && SeeingActor->IsA<ABaseItem>())
	{
		ABaseItem* Item = Cast<ABaseItem>(SeeingActor);
		Item->OnLostFocus();
	}

	//Calculate start and end locations
	FVector StartLocation = MainCharRef->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector EndLocation = StartLocation + (MainCharRef->GetFirstPersonCameraComponent()->GetForwardVector() * RaycastLength);


	//Ignore the owner of the raycast
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MainCharRef);

	FHitResult RaycastHitResult;
	const UWorld* World = GetWorld();

	//Performing the raycast
	World->LineTraceSingleByChannel(RaycastHitResult, StartLocation, EndLocation, ECC_WorldDynamic, QueryParams);

	//Just for Debug purposes
	if (bVisualizeRaycast) DrawDebugLine(World, StartLocation, EndLocation, FColor::Green, false, 1.5f);

	//Store the found actor
	SeeingActor = RaycastHitResult.GetActor();

	//If the found actor is a base item check if we need to switch the crosshair texture
	if (SeeingActor && SeeingActor->IsA<ABaseItem>())
	{
		ABaseItem* Item = Cast<ABaseItem>(SeeingActor);
		Item->OnBeginFocus();
		HUDRef->SetCrosshairTexture(Item->GetCrosshairTexture());
	}
	else HUDRef->SetCrosshairTexture(nullptr);
}

// Sets default values for this component's properties
UCharacterRaycastComponent::UCharacterRaycastComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCharacterRaycastComponent::BeginPlay()
{
	Super::BeginPlay();

	MainCharRef = Cast<AMainCharacter>(GetOwner());
}

// Called every frame
void UCharacterRaycastComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (MainCharRef)
	{
		SearchForUsableItems();

		/*const*/ UActionsTutorialComponent* TutorialComp = MainCharRef->GetActionsTutorialComponent();

		if (SeeingActor && SeeingActor->IsA<ABaseItem>())
		{
			TutorialComp->DisplayTutorialMessage(ETutorialMessageType::EUseItemTutorial);
		}
		
	}
}

