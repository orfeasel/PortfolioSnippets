// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Public/Environment/ActorSpawner.h"
#include "IntermissionNode.generated.h"



UCLASS()
class RNC_API AIntermissionNode : public AActor
{
	GENERATED_BODY()

private:

	UFUNCTION()
	void OnBoxOverlap(AActor* OverlapedActor, UPrimitiveComponent* OverlapedComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Sets default values for this actor's properties
	AIntermissionNode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/*The scene component that holds everything together as root*/
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComp;

	/*The Box Comp responsible for the overlap events with the boss. It determines if we need to 
	"raise" or "hide" the corresponding wall (smcomp)*/
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;
	
	/*The associated wall of the intermission node*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SMComp;

	/*The spawners that will be activated when the boss select this intermission node*/
	UPROPERTY(EditAnywhere, Category = "Intermission")
	TArray<AActorSpawner*> ActorSpawnerArray;

};
