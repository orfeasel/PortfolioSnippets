// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/Character.h"
#include "Public/Components/BackpackComponent.h"
#include "Public/Components/FootstepComponent.h"
#include "Public/Components/ActionsTutorialComponent.h"
#include "Public/Components/TimelineHandleComponent.h"
#include "Public/Controllers/MainCharacterController.h"
#include "MainCharacter.generated.h"

class UCharacterRaycastComponent;

UCLASS()
class PROJECTNA_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	AMainCharacterController* ControllerRef;

	bool bCanOpenBackpack = false;

	bool bJumpingInput = false;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void AddControllerYawInput(float Value) override;

	void AddControllerPitchInput(float Value) override;

	virtual void Jump() override;

	virtual void StopJumping() override;

	void Interact();

	UFUNCTION()
	void OnBackKeyPress();

	void HandleBackpackInput();

	UFUNCTION()
	void PerformCameraFade(float AlphaValue);

	void SetGamePaused(bool NewPauseStatus);

protected:

	/** Camera Component reference */
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPCameraComp;

	/** Raycast component reference */
	UPROPERTY(VisibleAnywhere)
	UCharacterRaycastComponent* FPRaycastComp;

	/** Backpack component reference */
	UPROPERTY(VisibleAnywhere)
	UBackpackComponent* BackpackComp;

	UPROPERTY(VisibleAnywhere)
	UFootstepComponent* FootstepComp;

	UPROPERTY(VisibleAnywhere)
	UActionsTutorialComponent* TutorialComp;

	UPROPERTY(VisibleAnywhere)
	UTimelineHandleComponent* TimelineHandleComp;


public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComp) override;

	/** Returns the first person camera component */
	FORCEINLINE UCameraComponent* GetFirstPersonCameraComponent() const { return FPCameraComp; }

	/** Returns the backpack component */
	FORCEINLINE UBackpackComponent* GetBackpackComponent() const { return BackpackComp; }

	FORCEINLINE UFootstepComponent* GetFootstepComponent() const { return FootstepComp; }

	FORCEINLINE UActionsTutorialComponent* GetActionsTutorialComponent() const { return TutorialComp; }

	UFUNCTION(BlueprintCallable, Category = ExitGame)
	void FinishGame();

	void ActivateBackpackupInput();

	void ActivateJumpInput();
};
