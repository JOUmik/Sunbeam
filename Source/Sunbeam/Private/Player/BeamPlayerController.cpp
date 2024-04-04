// Copyright Project SunBeam. All Rights Reserved.


#include "Player/BeamPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Player/BeamPawn.h"
#include "SunbeamGameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/StaticMeshActor.h"
#include "Items/Item.h"

void ABeamPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	EnhancedInputComponent->BindAction(RotateWithMouseInputAction, ETriggerEvent::Triggered, this, &ABeamPlayerController::RotateBeamWithMouseInput);
	EnhancedInputComponent->BindAction(RotateWithJoystickAction, ETriggerEvent::Triggered, this, &ABeamPlayerController::RotateBeamWithJoystick);
	EnhancedInputComponent->BindAction(SwitchBeamStateAction, ETriggerEvent::Completed, this, &ABeamPlayerController::SwitchBeamState);
	EnhancedInputComponent->BindAction(SwitchAction, ETriggerEvent::Started, this, &ABeamPlayerController::SwitchControlMethod);
	EnhancedInputComponent->BindAction(ChangeMapAction, ETriggerEvent::Started, this, &ABeamPlayerController::ChangeMapWithEnhancedInput);
	EnhancedInputComponent->BindAction(ChangeMirrorAction, ETriggerEvent::Started, this, &ABeamPlayerController::ShowMirrorWithEnhancedInput);
	EnhancedInputComponent->BindAction(ChangeMirrorAction, ETriggerEvent::Completed, this, &ABeamPlayerController::HideMirrorWithEnhancedInput);
	EnhancedInputComponent->BindAction(RotateLevelAction, ETriggerEvent::Started, this, &ABeamPlayerController::RotateLevelWithEnhancedInput);

}

void ABeamPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	GetWorldTimerManager().SetTimer(RotateTimeHandle, this, &ABeamPlayerController::BPReadDate, 0.01f, true);

	SunbeamGameInstance = Cast<USunbeamGameInstance>(GetGameInstance());
	if (SunbeamGameInstance) UseHardware = SunbeamGameInstance->HardwareControlEnabled;

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("RotateRoot"), OutActors);
	if (OutActors.Num() == 1)
	{
		RotateControlledActor = OutActors[0];
		TargetLevelRotation = RotateControlledActor->GetActorRotation();
	}
}

// Called every frame
void ABeamPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BPReadDate();

	if (ControlledLight)
	{
		ControlledLight->SetActorRotation(FMath::RInterpTo(ControlledLight->GetActorRotation(), TargetLightRotation, DeltaTime, LerpRate));
	}
	if (RotateControlledActor)
	{
		RotateControlledActor->SetActorRotation(FMath::RInterpTo(RotateControlledActor->GetActorRotation(), TargetLevelRotation, DeltaTime, LerpRate));
	}
}

void ABeamPlayerController::RotateBeamWithMouseInput(const FInputActionValue& InputActionValue)
{
	const FVector2D RotateAxisVector = InputActionValue.Get<FVector2D>();
	if (ABeamPawn* BeamPawn = Cast<ABeamPawn>(GetPawn()))
	{
		BeamPawn->RotateBeamPawn_MouseInput(RotateAxisVector);
	}
}

void ABeamPlayerController::RotateBeamWithJoystick(const FInputActionValue& InputActionValue)
{
	const FVector2D RotateAxisVector = InputActionValue.Get<FVector2D>();
	if (ABeamPawn* BeamPawn = Cast<ABeamPawn>(GetPawn()))
	{
		BeamPawn->RotateBeamPawn_ControllerInput(RotateAxisVector);
	}
}

void ABeamPlayerController::SwitchBeamState()
{
	if (ABeamPawn* BeamPawn = Cast<ABeamPawn>(GetPawn()))
	{
		BeamPawn->SwitchToNextBeamState();
	}
}

//Whether use hardware, hardware control is opened in default
void ABeamPlayerController::SwitchControlMethod() {
	UseHardware = !UseHardware;
	SunbeamGameInstance->HardwareControlEnabled = UseHardware;
	if (UseHardware) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hardware control open")));
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hardware control close")));
}

void ABeamPlayerController::ChangeMapWithEnhancedInput(const FInputActionValue& Value)
{
	if (UseHardware) return;

	float Input = Value.Get<float>();
	int32 Input_int = FMath::RoundToInt32(Input);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Map Index: %d"), Input_int));
	ChangeMap(Input_int - 1);
}

void ABeamPlayerController::ShowMirrorWithEnhancedInput(const FInputActionValue& Value)
{
	if (UseHardware) return;

	//if using NightLight, do not show focal
	if (EnabledLightIndex == 1) return;
	float Input = Value.Get<float>();
	int32 Input_int = FMath::RoundToInt32(Input) - 1;
	MirrorIndex = Input_int;
	ChangeMirror(true);
}

void ABeamPlayerController::HideMirrorWithEnhancedInput(const FInputActionValue& Value)
{
	//if using NightLight, do not show focal
	if (EnabledLightIndex == 1) return;
	ChangeMirror(false);
}

void ABeamPlayerController::RotateLevelWithEnhancedInput(const FInputActionValue& Value)
{
	if (UseHardware) return;

	float Input = Value.Get<float>();

	RotateLevel(Input);
}

void ABeamPlayerController::ChangeMapWithHardware(int index)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Map Index: %d"), index));
	ChangeMap(index);
}

void ABeamPlayerController::ChangeMirrorWithHardware(int index)
{
	if (MirrorIndex == index) return;
	MirrorIndex = index;
	GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Blue, FString::Printf(TEXT("Mirror Index: %d"), index));
	ChangeMirror(index != 0);
}

void ABeamPlayerController::RotateLevelWithHardware(int index)
{
	if(index > 0) index = FMath::Min(index, MaxRotateIndex);
	else index = FMath::Max(index, -MaxRotateIndex);

	const int RotateGap = index - RotateIndex;
	RotateLevel(RotateGap);
	
	RotateIndex = index;
	
	//GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Blue, FString::Printf(TEXT("Rotate Index: %d"), index));
}

void ABeamPlayerController::ChangeMap(int index)
{
	if (UGameInstance* GameIns = GetGameInstance())
	{
		Cast<USunbeamGameInstance>(GameIns)->ChangeMap(index);
	}
}

void ABeamPlayerController::RotateLevel(const int input)
{
	if (RotateControlledActor)
	{
		FRotator Rotation = TargetLevelRotation;
		const float RotateAngle = RotatePerAngle * abs(input);
		//ture right, negative
		if (input > 0)
		{
			if (RotateIndex >= 0 || FMath::Abs(RotateIndex) < MaxRotateIndex)
			{
				RotateIndex-= abs(input);
				Rotation.Yaw -= RotateAngle;
				TargetLevelRotation = Rotation;
			}
		}
		//ture left, positive
		else
		{
			if (RotateIndex <= 0 || RotateIndex < MaxRotateIndex)
			{
				RotateIndex+= abs(input);
				Rotation.Yaw += RotateAngle;
				TargetLevelRotation = Rotation;
			}
		}
	}
}
