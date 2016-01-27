// Fill out your copyright notice in the Description page of Project Settings.

#include "RoguelikeSurvival.h"
#include "AmmoBox.h"
#include "RoguelikeChar.h"

void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();

	//Sets up the right dimensions according to the static mesh used in this game

	StaticMesh->SetWorldScale3D(FVector(2.2, 2.2, 2.2));

	//Generates the right material for the ammo box based on the ammo type it provides
	SetAmmoBoxMaterialAndParticleEmitter();

	AmountOfTotalBullets = FMath::RandRange(LowerAmmoLimit, UpperAmmoLimit);
}

void AAmmoBox::SetAmmoBoxMaterialAndParticleEmitter()
{
	//Sets up the right material that the ammo box will have according to the type of ammo it provides
	switch (AmmoType)
	{
		case EAmmoType::Frost:
		{
			if (FrostAmmoBoxMaterial && FrostAmmoBoxEmmiterTemplate)
			{
				StaticMesh->SetMaterial(0, FrostAmmoBoxMaterial);

				ParticleEmitter->Template = FrostAmmoBoxEmmiterTemplate;
				ParticleEmitter->Activate(true);

			}
			break;
		}
		case EAmmoType::Incendiary:
		{
			if (IncendiaryAmmoBoxMaterial && IncendiaryAmmoBoxEmmiterTemplate)
			{
				StaticMesh->SetMaterial(0, IncendiaryAmmoBoxMaterial);

				ParticleEmitter->Template = IncendiaryAmmoBoxEmmiterTemplate;
				ParticleEmitter->Activate(true);
			}
			break;
		}
		default:
			break;
	}
}

void AAmmoBox::ActorOverlaped(AActor* Other)
{
	ARoguelikeChar* Character = Cast<ARoguelikeChar>(Other);
	if (Character && !this->IsPendingKill())
	{
		//Play Powerup SFX
		if (AudioComp)
		{
			AudioComp->Play();
		}
		Character->IncreaseBulletsLeft(AmmoType, AmountOfTotalBullets);


		this->Kill();
		GLog->Log("Bullets added");
	}
	
}
