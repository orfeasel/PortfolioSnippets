// Orfeas Eleftheriou 2016

#pragma once

#include "Components/ActorComponent.h"
#include "Public/Character/MainCharacter.h"
#include "Public/HUDs/MainCharacterHUD.h"
#include "CharacterRaycastComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNA_API UCharacterRaycastComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/** Main Character Reference */
	AMainCharacter* MainCharRef;

	/** The actor that the camera is currently pointing to */
	AActor* SeeingActor;

	/** HUD Reference */
	AMainCharacterHUD* HUDRef;

	/** Performs a line raycast to find any usable items in the direction the player is looking */
	void SearchForUsableItems();

public:	
	// Sets default values for this component's properties
	UCharacterRaycastComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	/** Returns the SeeingActor from the performed raycast */
	FORCEINLINE AActor* GetSeeingActor() { return SeeingActor; }

	/** Sets a new HUD reference */
	inline void SetHUDReference(AHUD* HUD) { HUDRef = Cast<AMainCharacterHUD>(HUD); }

protected:

	/** The length (in units) that the raycast is performed */
	UPROPERTY(EditAnywhere)
	float RaycastLength = 200.f;

	/** True if you want a visual representation of the performed raycast in the viewport */
	UPROPERTY(EditAnywhere)
	bool bVisualizeRaycast = false;

	
};
