// Orfeas Eleftheriou 2016

#pragma once

#include "Animation/AnimInstance.h"
#include "ButtlerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNA_API UButtlerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly)
	float MovementSpeed;

	UFUNCTION(BlueprintCallable, Category = ButtlerAnimInstace)
	void PlayFootstepSound();
	
public:

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	
};
