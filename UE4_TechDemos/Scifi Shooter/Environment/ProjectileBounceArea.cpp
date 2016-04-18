// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "ProjectileBounceArea.h"


// Sets default values
AProjectileBounceArea::AProjectileBounceArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AreaSM = CreateDefaultSubobject<UStaticMeshComponent>(FName("AreaSM"));

	SetRootComponent(AreaSM);
}

