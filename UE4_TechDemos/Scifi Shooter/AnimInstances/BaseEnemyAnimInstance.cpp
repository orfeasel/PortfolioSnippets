// Fill out your copyright notice in the Description page of Project Settings.

#include "RnC.h"
#include "BaseEnemyAnimInstance.h"



void UBaseEnemyAnimInstance::UpdatePawnSpeed()
{
	APawn* Pawn = TryGetPawnOwner();
	if (Pawn) CharacterSpeed = Pawn->GetVelocity().Size();
}

void UBaseEnemyAnimInstance::PlayAttackAnimation()
{
	if (AttackMontage && !Montage_IsPlaying(AttackMontage))
	{
		//GLog->Log("Playing attack montage");
		Montage_Play(AttackMontage);
	}
}

void UBaseEnemyAnimInstance::PlayGetHitAnimation()
{
	if (GetHitMontage && !Montage_IsPlaying(GetHitMontage))
	{
		//GLog->Log("Playing get hit montage");
		Montage_Play(GetHitMontage);
		//GLog->Log("Epaiksa montage");
	}
}
