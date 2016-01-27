// Fill out your copyright notice in the Description page of Project Settings.

#include "CavemanTestField.h"
#include "MyCharacter_FirstTry.h"
#include "IndoorActor.h"


// Sets default values
AIndoorActor::AIndoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Stats init.
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("IndoorComponent"));
	TriggerTemperature = 25;
	TemperatureMultiplier = 4;
}

// Called when the game starts or when spawned
void AIndoorActor::BeginPlay()
{
	Super::BeginPlay();

	//Registering overlap functions
	OnActorBeginOverlap.AddDynamic(this, &AIndoorActor::SetHeatDropRate);
	OnActorEndOverlap.AddDynamic(this, &AIndoorActor::SetHeatDropRateToDefault);
}

// Called every frame
void AIndoorActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AIndoorActor::SetHeatDropRate(AActor* OtherActor)
{
	AMyCharacter_FirstTry* Caveman = Cast<AMyCharacter_FirstTry>(OtherActor);
	//Setting up the new heat drop rate
	if (Caveman)
	{
		DefaultHeatDropRate = Caveman->Heat_DropRate;
		GLog->Log("Default heat drop rate:" + FString::SanitizeFloat(DefaultHeatDropRate));
		float HeatDropRate = (1 / TemperatureMultiplier) / TriggerTemperature;
		GLog->Log("Current HeatDropRate:" + FString::SanitizeFloat(HeatDropRate));
		Caveman->Heat_DropRate = HeatDropRate;
	}
	/*else
	{
		GLog->Log("No.");
	}*/
}

void AIndoorActor::SetHeatDropRateToDefault(AActor* OtherActor)
{
	AMyCharacter_FirstTry* Caveman = Cast<AMyCharacter_FirstTry>(OtherActor);
	//Resetting the heat drop rate to the default
	if (Caveman)
	{
		Caveman->Heat_DropRate = DefaultHeatDropRate;
	}

}