// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/BaseAIController.h"
#include "Public/Environment/IntermissionNode.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API ABossAIController : public ABaseAIController
{
	GENERATED_BODY()

private:

	/*True activates the intermission - false deactivates it*/
	void SetIntermissionPhaseStatus(bool Status);

	/*Holds a reference to all the intermission nodes on the current level*/
	TArray<AIntermissionNode*> IntermissionNodesArray;

	/*Finds the farthest intermission node based on the bosse's position
	and updates its blackboard value*/
	void FindTheFarthestrIntermissionNode();

	/*This array is used as a pair with the HealthPercentForAnIntermissionPhase array
	Together they determine the intermission phase that needs to be activated when the boss
	reaches a certain health percentage.
	Currently you can't expose a TMap to the UE4 editor so I think this approach on solving the current
	problem is a viable solution.*/
	TArray<bool> ActivatedIntermissionPhases;

	float IntermissionPhaseStartTime;

	float BossMaxHealth;

	bool bIsInIntermission;

	//--------------------------------------------------------------------
	//Blackboard Keys
	//--------------------------------------------------------------------
	const FName IntermissionPhaseKey = "IsInIntermissionPhase";

	const FName IntermissionNodeKey = "IntermissionNode";

	const FName IntermissionDurationKey = "IntermissionPhaseDuration";

protected:

	/*The Health percentages that the intermission phases will be activated.
	For example 2 entries of 33 and 66 mean that the boss will go to intermission phase
	when his health is at 33% and 66%. */
	UPROPERTY(EditAnywhere, Category = "Intermission")
	TArray<float> HealthPercentForAnIntermissionPhase;

	/*The duration of each intermission phase in seconds*/
	UPROPERTY(EditAnywhere, Category = "Intermission")
	float IntermissionDuration;
	
public:

	virtual void Possess(APawn* InPawn) override;

	/*Determines if the boss should go to intermission phase*/
	void PerformIntermissionCheck();

	/*Returns the current intermission node - always check for nullptr after using this!*/
	AIntermissionNode* GetCurrentIntermissionNode();
	
};
