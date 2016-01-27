// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "IndoorActor.generated.h"

UCLASS()
class CAVEMANTESTFIELD_API AIndoorActor : public AActor
{
	GENERATED_BODY()

private:
	/*The default heat drop rate that the character had before entering the heat zone*/
	float DefaultHeatDropRate;
	
public:	
	// Sets default values for this actor's properties
	AIndoorActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/*The indoor collider*/
	UPROPERTY(VisibleAnywhere, Category = "IndoorTrigger")
	UBoxComponent* BoxComp;
	
	/*The Temperature of trigger in Celcius (ie 25)*/
	UPROPERTY(EditAnywhere, Category = "HeatMechanic")
	int32 TriggerTemperature;

	/*The Calculation formula is: HeatDropRate = (1 / TemperatureMultiplier) / TriggerTemperature*/
	UPROPERTY(EditAnywhere, Category = "HeatMechanic")
	float TemperatureMultiplier;

	/*Sets the new heat drop rate*/
	//UFUNCTION(BlueprintCallable, Category = "HeatMechanic")
	UFUNCTION()
	void SetHeatDropRate(AActor* OtherActor);

	//UFUNCTION(BlueprintCallable, Category = "HeatMechanic")
	UFUNCTION()
	void SetHeatDropRateToDefault(AActor* OtherActor);
};
