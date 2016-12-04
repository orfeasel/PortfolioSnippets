// Orfeas Eleftheriou 2016

#pragma once

#include "Components/ActorComponent.h"
#include "FootstepComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNA_API UFootstepComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	USoundBase* GetSurfaceFootstepSound();

	UAudioComponent* AudioComponentRef;

protected:

	UPROPERTY(EditAnywhere)
	float VerticalRaycastMultiplier = -1.1f;

	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> WalkableSurfaces;

public:	

	// Sets default values for this component's properties
	UFootstepComponent();

	void PlayFootstepSound();

	/** Sets a new audio component. If the footstep component has an assigned audio component 
	* it will play all the footstep sounds from the attached component
	* @param NewAudioComponent - an audio component
	*/
	FORCEINLINE void SetAttachedAudioComponent(UAudioComponent* NewAudioComp)
	{
		AudioComponentRef = NewAudioComp;
	}
	
};
