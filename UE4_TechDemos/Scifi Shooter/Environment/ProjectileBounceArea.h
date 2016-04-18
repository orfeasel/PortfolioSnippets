// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ProjectileBounceArea.generated.h"

UCLASS()
class RNC_API AProjectileBounceArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBounceArea();

	/*The static mesh of the wall*/
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* AreaSM;

};
