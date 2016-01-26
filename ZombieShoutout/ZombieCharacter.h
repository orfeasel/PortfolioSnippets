// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "RoguelikeChar.h"
#include "ZombieAnimInstance.h"
#include "ZombieCharacter.generated.h"

//DECLARE_DYNAMIC_DELEGATE_OneParam(CustomDelegate, float);
//DECLARE_DELEGATE_OneParam(MyOwnDelegate, float);

UCLASS()
class ROGUELIKESURVIVAL_API AZombieCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/*Zombie's Anim Instance*/
	UZombieAnimInstance* ZAnimInstance;

	/*Character reference*/
	ARoguelikeChar* OurCharacter;

	/*The randomized health value of the zombie*/
	float CurrentHealth;

	/*The randomized speed value of the zombie*/
	float CurrentSpeed;

	/*Stops the Controller and Plays the Death animation of the zombie*/
	void Die();

	/*Handles both the necessary damage and damage effects that must be applied to the zombie*/
	void HandleDamage(float DamageAmount,FDamageEvent DamageEvent);

	/*Applies given damage to the zombie*/
	UFUNCTION()
	void ApplyDamage(float DamageAmount);

	
	/*True if the zombie has a dot effect active*/
	bool bHasDot;
	/*True if the zombie has a slow effect active*/
	bool bHasSlow;

	/************************************************************************/
	/* Incendiary Ammo region                                                */
	/************************************************************************/

	/*The TimerHandle responsible for the dot effect*/
	FTimerHandle DotTimerHandle;

	/*The time (in seconds) that the dot effect applied to the zombie*/
	float DotApplicationTime;

	/*Applies a DOT to the zombie. Kills the slow effect if enabled*/
	void ApplyDotEffect(float DamageAmount);

	/*Stops the DOT Effect if needed.*/
	void DisableDotEffectIfNecessary(bool ForceStop = false);

	/************************************************************************/
	/* Frost Ammo region                                                    */
	/************************************************************************/

	/*The TimerHandle responsible for the slow effect*/
	FTimerHandle SlowTimerHandle;
	
	/*Apples a slow effect to the zombie. Kills the dot effect if enabled*/
	void ApplySlowEffect();

	/*Stores the initialized max walk speed*/
	float InitialMaxWalkSpeed;

	/*Disables the slow effect.*/
	void DisableSlowEffect();

	/************************************************************************/
	/* Death                                                                */
	/************************************************************************/

	FTimeline AlphaReductionTimeline;

	/*Handles the timeline progress for each timeline tick*/
	UFUNCTION()
	void HandleTimelineProgress(float AlphaReduction);

	/*Kills the zombie - fires when the timeline is finished*/
	/*UFUNCTION()
	void KillZombie();*/

public:
	// Sets default values for this character's properties
	AZombieCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/*************************Stats****************************/

	/*The least possible amount of health that the zombies will have
	The generated health values are >=MinHealth and <=MaxHealth*/
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MinHealth;

	/*The max possible amount of health that the zombies will have
	The generated health values are >=MinHealth and <=MaxHealth*/
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth;

	/*Returns true if the zombie is alive*/
	FORCEINLINE bool IsAlive() { return CurrentHealth > 0; }

	/*The least possible move speed of the zombie*/
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MinSpeed;

	/*The max possible move speed of the zombie*/
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxSpeed;

	/*Zombie minimum damage*/
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MinDamage;

	/*Zombie maximum damage*/
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MaxDamage;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float MeleeDistanceThreshold;

	void StatsInit(float MinHealth, float MaxHealth, float MinDamage, float MaxDamage);

	/************************************************************************/
	/* Combat                                                               */
	/************************************************************************/

	/*Zombie Damage*/
	UPROPERTY(EditAnywhere, Category = "Combat")
	float Damage;

	/*Zombie Attack*/
	void Attack();

	/*Responsible for the incoming damage. Applies the damage and necessary effects*/
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/*The radius of the raycast sphere in attack animation*/
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackRaycastRadius;

	/*The time (in seconds) that the dot effect ticks*/
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float DotTickInSeconds;

	/*The slow effect of frost bullets in percentage.
	Meaning a value of 0.8 is 80% slow effect.
	WARNING: Enter a value between 0 and 1*/
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float SlowPercentage;

	/*Slow Effect duration in seconds*/
	UPROPERTY(EditAnywhere, Category = "Combat")
	float SlowEffectDuration;

	/************************************************************************/
	/* Animations                                                           */
	/************************************************************************/

	FORCEINLINE UZombieAnimInstance* GetZombieAnimInstance() { return ZAnimInstance; }

	/*Reduces the alpha channel of the skeletal mesh of the zombie*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
	void AlphaChannelReduction();

	/*****************************AI*****************************/
	/*The corresponding behavior tree that will drive the zombie*/
	UPROPERTY(EditAnywhere,Category="AI")
	class UBehaviorTree* BehaviorTree;

	/************************************************************************/
	/* SFX                                                                  */
	/************************************************************************/
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* AudioComponent;

	/************************************************************************/
	/* Visual stuff                                                         */
	/************************************************************************/

	/*The particle system responsible for the visual effect of debuffs*/
	UPROPERTY(AdvancedDisplay,VisibleAnywhere,Category="Visuals")
	UParticleSystemComponent* ParticleSystemComponent;

	/*The frost effect for the zombies*/
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UParticleSystem* SlowEffectParticleTemplate;

	/*The fire effect for the zombies*/
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	UParticleSystem* FireEffectParticleTemplate;

	/************************************************************************/
	/* Death                                                                */
	/************************************************************************/

	/*The curve float reference for the alpha reduction*/
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* AlphaReductionCurveFloat;

	/*The material instance reference when the zombie's dead*/
	UPROPERTY(EditDefaultsOnly)
	UMaterialInstance* DeathMaterialInstance;
};
