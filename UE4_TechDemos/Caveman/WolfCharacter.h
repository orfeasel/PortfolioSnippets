// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AnimInstances/WolfAnimInstance.h"
#include "EnemyStatsComponent.h"
#include "MyCharacter_FirstTry.h"
#include "WolfCharacter.generated.h"


UENUM(BlueprintType)
enum class EWolfActions : uint8
{
	Idle,
	Chase,
	Trot,
	Escape
};

UCLASS()
class CAVEMANTESTFIELD_API AWolfCharacter : public ACharacter
{
	GENERATED_BODY()

	//Handles the Animation Blueprint
	void UpdateCorrespondingAnimation();


public:
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	/*--------------------AI--------------------*/
	//Component for pawn sensing
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	/* Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SenseTimeOut;

	/*Triggered when wolf sees the player*/
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	/*Triggered when wolf hears the player*/
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	/*True if wolf has a target*/
	bool bHasSensedTarget;

	/*Last seen time of wolfs target*/
	float LastSeenTime;
	/*--------------------AI--------------------*/

	/*------------------Combat------------------*/

	/*Seconds between consecutive attacks.*/
	float MeleeHitCD;

	/*Returns true if the attack was successfull*/
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PerformMeleeHit();

	bool CanPerformAttack();

	float LastHitTime;

	/*Actor who has hit the wolf*/
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	AActor* HitActor;

	/*Handles the get hurt animation, sets the actor who hit the wolf and calculates dmg (todo)*/
	void GetHit(APawn* ActorWhoPerformedTheHit);

	/*Used in order to block random howls.*/
	AActor* PreviousSensedTarget;

	/*True if is in melee range of the character*/
	bool bIsInMeleeRange;

	

	UFUNCTION(BlueprintCallable, Category="Combat")
	void OnMeleeColliderOverlapStart(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category="Combat")
	void OnMeleeColliderOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	/*------------------Combat------------------*/
	

	UWolfAnimInstance* MyAnimInstance;

	// Sets default values for this character's properties
	AWolfCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UEnemyStatsComponent* Stats;

	UPawnNoiseEmitterComponent* NoiseEmitterComp;
	
	void EnableHunting(AMyCharacter_FirstTry* SensedPawn);
};
