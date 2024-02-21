// Copyright Project SunBeam. All Rights Reserved.


#include "Player/BeamPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player/BeamPawn.h"

void ABeamPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ABeamPlayerController::RotateBeam);
	EnhancedInputComponent->BindAction(SwitchBeamStateAction, ETriggerEvent::Completed, this, &ABeamPlayerController::SwitchBeamState);
}

void ABeamPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void ABeamPlayerController::RotateBeam(const FInputActionValue& InputActionValue)
{
	const FVector2D RotateAxisVector = InputActionValue.Get<FVector2D>();
	if (ABeamPawn* BeamPawn = Cast<ABeamPawn>(GetPawn()))
	{
		BeamPawn->RotateBeamPawn_MouseInput(RotateAxisVector);
	}
}

void ABeamPlayerController::SwitchBeamState(const FInputActionValue& InputActionValue)
{
	if (ABeamPawn* BeamPawn = Cast<ABeamPawn>(GetPawn()))
	{
		BeamPawn->SwitchToNextBeamState();
	}
}
