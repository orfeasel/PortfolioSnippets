// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "RnC.h"
#include "RnCCharacter.h"
#include "DrawDebugHelpers.h"
#include "Public/Weapons/RangeWeapon.h"
#include "Public/Controllers/RnCPlayerController.h"
//#include "Public/Misc/RnCGameInstance.h"
//////////////////////////////////////////////////////////////////////////
// ARnCCharacter

//-------------------------------------------------------------------------------------
//Creates a new weapon binding. Use this when adding a new weapon to the character
//-------------------------------------------------------------------------------------
void ARnCCharacter::CreateNewWeaponEquipBinding(FName ActionBinding, EWeaponType WeaponTypeToEquip)
{
	FInputActionBinding EquipWeaponActionBinding;

	EquipWeaponActionBinding.ActionName = ActionBinding;
	EquipWeaponActionBinding.KeyEvent = IE_Pressed;

	FInputActionHandlerSignature EquipWeaponActionHandler;

	EquipWeaponActionHandler.BindUFunction(WeaponsComp, FName("UpdateCharacterEquippedWeapon"), WeaponTypeToEquip);

	EquipWeaponActionBinding.ActionDelegate = EquipWeaponActionHandler;

	InputComponent->AddActionBinding(EquipWeaponActionBinding);
}

//-------------------------------------------------------------------------------------
//Attaches on a jump node
//-------------------------------------------------------------------------------------
void ARnCCharacter::PerformJumpOnNode()
{
	if(JumpNode)
	{
		JumpNode->ApplyLerp(this);
	}
}

//-------------------------------------------------------------------------------------
//Creates a closed area using the character's position as its center and enables
//the bouncing effect on all the projectiles for the lifespan of the area.
//-------------------------------------------------------------------------------------
void ARnCCharacter::CreateProjectileBounceArea()
{
	if(BounceAreaBP)
	{
		//The spawn params that are used in all spawns
		FActorSpawnParameters SpawnParams;

		DECLARE_DELEGATE_TwoParams(BounceAreaSpawner, FVector, FRotator);

		BounceAreaSpawner Delegate;

		//Spawns a wall based on the relative location and rotation given
		Delegate.BindLambda([&](FVector WallRelativeLocation, FRotator WallRelativeRotation)
		{
			//Take into consideration the location of the player in order to spawn the walls around him
			WallRelativeLocation += GetActorLocation();
			WallRelativeRotation += GetActorRotation();

			//Create the desired transform
			FTransform WallTransform;
			WallTransform.SetLocation(WallRelativeLocation);
			WallTransform.SetRotation(WallRelativeRotation.Quaternion());

			//Spawn the wall and apply the desired scale
			AProjectileBounceArea* BounceArea = GetWorld()->SpawnActor<AProjectileBounceArea>(BounceAreaBP, WallTransform, SpawnParams);

			if (BounceArea)
			{
				BounceArea->SetActorScale3D(UnifiedScaleMultiplier);

				BounceArea->SetLifeSpan(LifeTimeOfBounceArea);
			}

		});

		if(Delegate.IsBound())
		{
			//Enable the battle cry animation
			if (MainCharAnimInstance)
			{
				bIsInBattleCry = true;
				MainCharAnimInstance->EnableBattlecry();
			}

			//Enabling projectile bouncing effect
			
			bShouldProjectilesBounce = true;

			//Disable the projectiles bouncing after a certain amount of time
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]()
			{
				bShouldProjectilesBounce = false;
			});
			
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, LifeTimeOfBounceArea, false);

			//We're going to use the following vectors in order to spawn the walls in the desired rotation and location
			FVector ForwardActorVector = GetActorForwardVector();
			FVector RightActorVector = GetActorRightVector();

			//Spawning the wall that the player will face
			Delegate.Execute(ForwardActorVector * ForwardVectorMultiplier, FRotator::ZeroRotator);
			
			//Spawning the wall right behind the player
			Delegate.Execute((ForwardActorVector * -1) * ForwardVectorMultiplier, FRotator::ZeroRotator);

			//Spawning the wall on the right side of the player
			Delegate.Execute(RightActorVector * RightVectorMultiplier, FRotator(0, 90.f, 0));

			//Spawning the wall on the left side of the player
			Delegate.Execute((RightActorVector * -1) * RightVectorMultiplier, FRotator(0, 90.f, 0));

			//Spawning the "ceiling" wall
			Delegate.Execute(GetActorUpVector() * UpVectorMultiplier, FRotator(90.f,0,0));

		}
	}
}

ARnCCharacter::ARnCCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AutomaticRotationRate = 10.f;

	//Weapons component init.
	WeaponsComp = CreateDefaultSubobject<UWeaponsComponent>(FName("WeaponsComponent"));

	//Stats init.
	Health = 100.f;

	bIsFiring = false;
	bShouldProjectilesBounce = false;

	PickupDistance = 20.f;

	JumpNode = nullptr;
	
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//-------------------------------------------------------------------------------------
//Enables the boss fight interface
//-------------------------------------------------------------------------------------
void ARnCCharacter::EnableBossUI()
{
	if(MainCharController)
	{
		MainCharController->EnableBossUI();
	}
}

//-------------------------------------------------------------------------------------
//Disables the battle cry animation flag
//-------------------------------------------------------------------------------------
void ARnCCharacter::DisableBattlecryAnimation()
{
	bIsInBattleCry = false;
}

void ARnCCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Hold a reference to our anim instance for future manipulations
	MainCharAnimInstance = Cast<UMainCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	//Initialize the equipped weapon
	CurrentlyEquippedWeapon = nullptr;

	//Spawn a knife as a default weapon
	WeaponsComp->UpdateCharacterEquippedWeapon(EWeaponType::Knife);

	//Getting a reference of the controller to avoid casting types later
	ARnCPlayerController* Con = Cast<ARnCPlayerController>(GetController());
	if (Con)
	{
		MainCharController = Con;
		//Initial Update
		MainCharController->UpdateUI();
	}
}

void ARnCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MainCharAnimInstance && CurrentlyEquippedWeapon)
	{
		if (MainCharAnimInstance->IsMeleeMontagePlaying() && CurrentlyEquippedWeapon->IsMelee())
		{
			TArray<FHitResult> HitActors;
			bool HasHitAnyActors = CurrentlyEquippedWeapon->GetHitActors(HitActors);

			//SetDamageRaycastStatus(false);
		}
		else if (bIsFiring)
		{
			ARangeWeapon* RangeWeapon = Cast<ARangeWeapon>(CurrentlyEquippedWeapon);
			if (RangeWeapon)
			{
				RangeWeapon->Fire();

				MainCharController->UpdateUI();
			}
		}

	}
}

//--------------------------------------------------------------------------------------
//Increases Ammo on the given weapon
//--------------------------------------------------------------------------------------
void ARnCCharacter::IncreaseAmmo(int32 AmmoToAdd, EWeaponType WeaponType)
{
	if (WeaponsComp && MainCharController)
	{
		WeaponsComp->UpdateWeaponAmmo(WeaponType, AmmoToAdd);
		MainCharController->UpdateUI();
	}
}


//--------------------------------------------------------------------------------------
//Assign a new equipped weapon on the character
//--------------------------------------------------------------------------------------
void ARnCCharacter::SetNewEquippedWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		CurrentlyEquippedWeapon = WeaponToEquip;
		//Sometimes when the game loads the controller delays to posses the character,
		//that's why we have an if statement in here
		if (MainCharController) MainCharController->UpdateUI();
	}
}

//-------------------------------------------------------------------------------------
//Enables the damage raycast for the equipped melee weapon
//-------------------------------------------------------------------------------------
void ARnCCharacter::EnableWeaponRaycasting()
{
	if (CurrentlyEquippedWeapon && CurrentlyEquippedWeapon->IsMelee()) CurrentlyEquippedWeapon->SetRaycasting(true);
}

//-------------------------------------------------------------------------------------
//Returns true if a melee weapon is equipped - false otherwise
//-------------------------------------------------------------------------------------
bool ARnCCharacter::IsMeleeWeaponEquipped()
{
	if (CurrentlyEquippedWeapon) return CurrentlyEquippedWeapon->IsMelee();

	return false;

}

//-------------------------------------------------------------------------------------
//Increases the health of the player by a given Value. Makes sure to clamp to max hp
//Returns true if successful
//-------------------------------------------------------------------------------------
bool ARnCCharacter::IncreaseHealthBy(float HealthToAdd)
{
	if(Health<MAX_HEALTH_VALUE)
	{
		(Health + HealthToAdd > MAX_HEALTH_VALUE) ? Health = MAX_HEALTH_VALUE : Health += HealthToAdd;
		if (MainCharController) MainCharController->UpdateUI();
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------
//Returns the current ammo of the equipped weapon
//-------------------------------------------------------------------------------------
int32 ARnCCharacter::GetAmmoLeft()
{
	if (CurrentlyEquippedWeapon)
	{
		return CurrentlyEquippedWeapon->GetCurrentAmmo();
	}
	else return 0;
}

//-------------------------------------------------------------------------------------
//Perform a melee attack or continue an existing combo move
//-------------------------------------------------------------------------------------
void ARnCCharacter::PerformMeleeAttack()
{
	if (MainCharAnimInstance && CurrentlyEquippedWeapon && CurrentlyEquippedWeapon->IsMelee())
	{
		//If the player is eligible to perform an attack do so....

		if (!MainCharAnimInstance->IsMeleeMontagePlaying())
		{
			MainCharAnimInstance->Attack();

			CurrentlyEquippedWeapon->ResetComboMoves();
		}
		else
		{
			//Enables melee combo if the player press the attack buttons at the right time
			EnableWeaponRaycasting();

			CurrentlyEquippedWeapon->IncreaseComboMove();

			MainCharAnimInstance->ContinueMeleeCombo();
		}
	}
}

float ARnCCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= DamageAmount;

	//Calculate the new health value and update the UI in any case

	MainCharController->UpdateUI();

	if (Health > 0)
	{
		MainCharAnimInstance->PlayGetHitAnimation();
	}
	else
	{
		MainCharAnimInstance->PlayDeathAnimation();
		StopFire();
		MainCharController->UnPossess();
	}

	return ActualDamage;
}


//////////////////////////////////////////////////////////////////////////
// Input
void ARnCCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);

	//Action Bindings
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Binding the (stop)fire/melee keys

	InputComponent->BindAction("Melee", IE_Pressed, this, &ARnCCharacter::PerformMeleeAttack);

	InputComponent->BindAction("Fire", IE_Pressed, this, &ARnCCharacter::Fire);
	InputComponent->BindAction("StopFire", IE_Released, this, &ARnCCharacter::StopFire);


	//Binding the weapon equip functionality

	CreateNewWeaponEquipBinding(FName("Knife_Equip"), EWeaponType::Knife);
	CreateNewWeaponEquipBinding(FName("Rifle_Equip"), EWeaponType::Rifle);
	CreateNewWeaponEquipBinding(FName("Shotgun_Equip"), EWeaponType::Shotgun);

	InputComponent->BindAction(FName("JumpNodeAttach"), IE_Pressed, this, &ARnCCharacter::PerformJumpOnNode);

	InputComponent->BindAction(FName("BouncingArea"), IE_Pressed, this, &ARnCCharacter::CreateProjectileBounceArea);

	//Binding the Pause key functionality

	FInputActionBinding EscKey;

	EscKey.ActionName = FName("Pause");
	EscKey.bExecuteWhenPaused = true;
	EscKey.KeyEvent = IE_Pressed;

	ARnCPlayerController* Con = Cast<ARnCPlayerController>(GetController());
	if (Con) EscKey.ActionDelegate.BindDelegate(Con, FName("HandlePauseState"));

	InputComponent->AddActionBinding(EscKey);

	InputComponent->BindAxis("MoveForward", this, &ARnCCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARnCCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ARnCCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ARnCCharacter::LookUpAtRate);

}

void ARnCCharacter::Fire()
{
	if (!CurrentlyEquippedWeapon->IsMelee())
	{
		bIsFiring = true;
	}
}

void ARnCCharacter::StopFire()
{
	bIsFiring = false;
}

#pragma region Movement
void ARnCCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARnCCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARnCCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bIsInBattleCry)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARnCCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bIsInBattleCry)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
#pragma endregion Movement and Inputs functionality