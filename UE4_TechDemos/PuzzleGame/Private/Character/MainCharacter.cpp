// Orfeas Eleftheriou 2016

#include "ProjectNA.h"
#include "MainCharacter.h"
#include "Public/Components/CharacterRaycastComponent.h"
#include "Public/Items/BaseItem.h"
#include "Public/Controllers/MainCharacterController.h"

void AMainCharacter::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMainCharacter::AddControllerYawInput(float Value)
{
	if (Value != 0.f && ControllerRef && ControllerRef->IsAbleToInteract())
	{
		ControllerRef->AddYawInput(Value);
	}
}

void AMainCharacter::AddControllerPitchInput(float Value)
{
	if (Value != 0.f && ControllerRef && ControllerRef->IsAbleToInteract())
	{
		ControllerRef->AddPitchInput(Value);
	}
}

void AMainCharacter::Jump()
{
	if (bJumpingInput) Super::Jump();
}

void AMainCharacter::StopJumping()
{
	if (bJumpingInput) Super::StopJumping();
}

void AMainCharacter::Interact()
{
	ABaseItem* SeeingActor = Cast<ABaseItem>(FPRaycastComp->GetSeeingActor());
	if (SeeingActor && ControllerRef->IsAbleToInteract())
	{
		SeeingActor->Interact(*BackpackComp);
	}
}

void AMainCharacter::OnBackKeyPress()
{
	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		SetGamePaused(false);
	}
	else if(ControllerRef->IsReadingADocument())
	{
		ControllerRef->CloseDocument();
	}
	else
	{
		SetGamePaused(true);
	}
}

void AMainCharacter::HandleBackpackInput()
{
	if(bCanOpenBackpack) ControllerRef->HandleBackpackInput();
}

void AMainCharacter::PerformCameraFade(float AlphaValue)
{
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	CameraManager->SetManualCameraFade(AlphaValue, FColor::Black, true);
}

void AMainCharacter::SetGamePaused(bool NewPauseStatus)
{
	UGameplayStatics::SetGamePaused(GetWorld(), NewPauseStatus);

	ControllerRef->SetGamePausedStatus(NewPauseStatus);
}

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FPCameraComp = CreateDefaultSubobject<UCameraComponent>(FName("FPCameraComp"));

	FPCameraComp->SetupAttachment(GetRootComponent());

	//Add an offset of the camera (this was done by trial and error through the Blueprint's viewport)
	FPCameraComp->AddLocalOffset(FVector(0, 0, 70));

	FPCameraComp->bUsePawnControlRotation = true;

	FPRaycastComp = CreateDefaultSubobject<UCharacterRaycastComponent>(FName("FPRaycastComp"));

	BackpackComp = CreateDefaultSubobject<UBackpackComponent>(FName("BackpackComp"));

	FootstepComp = CreateDefaultSubobject<UFootstepComponent>(FName("FootstepComp"));

	TutorialComp = CreateDefaultSubobject<UActionsTutorialComponent>(FName("TutorialComp"));

	TimelineHandleComp = CreateDefaultSubobject<UTimelineHandleComponent>(FName("TimelineHandleComp"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	FPRaycastComp->SetHUDReference(Cast <APlayerController>(GetController())->GetHUD());

	ControllerRef = Cast<AMainCharacterController>(GetController());

	//Disable input - we'll reactivate it when the introboat reaches the port
	//DisableInput(ControllerRef);

	//Preparing for fade in animation

	TimelineHandleComp->SetInitialAndTargetValues(1.f, 0.f);

	TimelineHandleComp->BindFloatCurveFloatExecutionFunction(this, FName("PerformCameraFade"));

	//Performing the fade in
	TimelineHandleComp->PlayTimeline();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(class UInputComponent* InputComp)
{
	Super::SetupPlayerInputComponent(InputComp);

	check(InputComp);

	//Action mappings

	//InputComp->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	//InputComp->BindAction(FName("Jump"), IE_Released, this, &ACharacter::StopJumping);

	InputComp->BindAction(FName("Jump"), IE_Pressed, this, &AMainCharacter::Jump);
	InputComp->BindAction(FName("Jump"), IE_Released, this, &AMainCharacter::StopJumping);

	InputComp->BindAction(FName("Interact"), IE_Pressed, this, &AMainCharacter::Interact);

	FInputActionBinding BackActionBinding;

	BackActionBinding.ActionName = FName("Back");
	BackActionBinding.ActionDelegate.BindDelegate(this, FName("OnBackKeyPress"));
	BackActionBinding.bExecuteWhenPaused = true;
	BackActionBinding.KeyEvent = IE_Pressed;

	InputComp->AddActionBinding(BackActionBinding);

	//InputComp->BindAction(FName("Back"), IE_Pressed, this, &AMainCharacter::OnBackKeyPress);

	InputComp->BindAction(FName("Backpack"), IE_Pressed, this, &AMainCharacter::HandleBackpackInput);

	//Axis mappings

	InputComp->BindAxis(FName("MoveForward"), this, &AMainCharacter::MoveForward);
	InputComp->BindAxis(FName("MoveRight"), this, &AMainCharacter::MoveRight);

	InputComp->BindAxis(FName("Turn"), this, &AMainCharacter::AddControllerYawInput);
	InputComp->BindAxis(FName("LookUp"), this, &AMainCharacter::AddControllerPitchInput);
}

void AMainCharacter::FinishGame()
{
	TimelineHandleComp->SetInitialAndTargetValues(0.f, 1.f);

	TimelineHandleComp->BindFloatCurveFloatExecutionFunction(this, FName("PerformCameraFade"));

	//Performing the fade in
	TimelineHandleComp->PlayTimeline();

	(Cast<AMainCharacterHUD>(ControllerRef->GetHUD()))->DisableHUDDrawing();

	DisableInput(ControllerRef);
	
	TutorialComp->SetTutorialMessageDisplaySeconds(10.f);
	TutorialComp->DisplayTutorialMessage(ETutorialMessageType::EGameFinish);

	FTimerHandle TimerHandle;

	FTimerDelegate TimerDel;

	TimerDel.BindLambda([&]()
	{
		FGenericPlatformMisc::RequestExit(false);
	});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 10.f, false);
}

void AMainCharacter::ActivateBackpackupInput()
{
	bCanOpenBackpack = true;
}

void AMainCharacter::ActivateJumpInput()
{
	bJumpingInput = true;
}

