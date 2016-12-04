// Orfeas Eleftheriou 2016

#pragma once

#include "Animation/AnimInstance.h"
#include "MainCharAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API UMainCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly)
	bool bIsMoving;

	UFUNCTION(BlueprintCallable, Category = MainCharAnimInstance)
	void PlayFootstepSound();

public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
