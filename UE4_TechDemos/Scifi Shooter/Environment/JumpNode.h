// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "JumpNode.generated.h"

UCLASS()
class RNC_API AJumpNode : public AActor
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void ActorBeginOverlap(AActor* OverlapedActor);

	UFUNCTION()
	void ActorEndOverlap(AActor* OtherActor);

	/*Holds a reference to the timeline we want to use*/
	FTimeline InterpTimeline;

	/*Advances the timeline by deltatime*/
	UFUNCTION()
	void AdvanceTimeline(float DeltaTime);

	/*Actor that it's going to get interpolated*/
	AActor* InterpolatingActor;

	/*The starting location of the actor that we will interpolate*/
	FVector StartingLocationOfInterpolationActor;

	/*The Particle component that will get spawned when the player attaches to the jump node*/
	UParticleSystemComponent* ParticleComp;
	
public:	
	// Sets default values for this actor's properties
	AJumpNode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	/*Intepolates the location of the given actor*/
	void ApplyLerp(AActor* ActorToInterpolate);

protected:

	/*The Box component is used in order to determine if any actor is in range to use the jump node*/
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComp;

	/*The static mesh of the jump node*/
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* JumpNodeSM;

	/*The Interpolation curve*/
	UPROPERTY(EditDefaultsOnly)
	UCurveFloat* InterpolationCurve;

	/*The Particle System that gets spawned when "lerping" the location of the playable character*/
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ArcParticle;

	/*When the Actor's distance <= InterpolationBreakDistance from the Static Mesh of the jump node, the pull effect stops*/
	UPROPERTY(EditAnywhere,Category="InterpolationProperties")
	float InterpolationBreakDistance;

};
