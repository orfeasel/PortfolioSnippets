// Orfeas Eleftheriou 2016

#pragma once

#include "GameFramework/Actor.h"
#include "Public/Components/TimelineHandleComponent.h"
#include "ScriptedDoor.generated.h"

UCLASS()
class PROJECTNA_API AScriptedDoor : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly/*, Category = ScriptedProperties*/)
	float OpenYaw = 90.f;

	UPROPERTY(VisibleAnywhere)
	UTimelineHandleComponent* TimelineHandleComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorSM;

	UFUNCTION()
	void Rotate(FRotator NewRotation);

	UPROPERTY(EditAnywhere)
	USoundBase* DoorSlamSFX;
	
public:	
	// Sets default values for this actor's properties
	AScriptedDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = CustomScripts)
	void LockDoor();
	
	UFUNCTION(BlueprintImplementableEvent, Category = CustomScripts)
	void UnlockDoor();
};
