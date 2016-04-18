// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TargetPoint.h"
#include "AIPatrolNode.generated.h"

/**
 * 
 */
UCLASS()
class RNC_API AAIPatrolNode : public ATargetPoint
{
	GENERATED_BODY()
	
private:
	/*True when the AI occupies the current node*/
	bool bIsOccupied;

public:

	AAIPatrolNode();

	/*Set a new occupy stauts on the current node*/
	void SetOccupyStatus(bool NewStatus);
	
	FORCEINLINE bool IsOccupied() { return bIsOccupied; }
};
