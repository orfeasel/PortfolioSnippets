// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/Character.h"
#include "Components/FootstepComponent.h"
#include "ButtlerCharacter.generated.h"

UCLASS()
class PROJECTNA_API AButtlerCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere)
	UFootstepComponent* FootstepComp;

	UPROPERTY(EditAnywhere)
	AActor* ActorToMoveAt;

	/** The actor that the buttler is going to face when he completes his movement to ActorToMoveAt */
	UPROPERTY(EditAnywhere)
	AActor* ActorToFace;

	UPROPERTY(EditAnywhere)
	AActor* ExitActor;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* FootstepAudioComp;

	UPROPERTY(EditAnywhere)
	USoundBase* WelcomeSFX;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* TalkingAudioComp;

public:
	// Sets default values for this character's properties
	AButtlerCharacter();

	FORCEINLINE UFootstepComponent* GetFootstepComponent() const { return FootstepComp; };

	void WalkToCabin();

	UFUNCTION(BlueprintCallable, Category = ButtlerAI)
	void InitializeExit();
};
