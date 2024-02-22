// Copyright Project SunBeam. All Rights Reserved.


#include "ControlPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SunbeamGameInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/StaticMeshActor.h"
#include "Items/Item.h"

// Sets default values
AControlPawn::AControlPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AControlPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	LightsInitialize();
	ControlledActorsInitialize();
	
	GetWorldTimerManager().SetTimer(RotateTimeHandle, this, &AControlPawn::BPReadDate, 0.09f, true);

	SunbeamGameInstance = Cast<USunbeamGameInstance>(GetGameInstance());
	if(SunbeamGameInstance) UseHardware = SunbeamGameInstance->HardwareControlEnabled;
}

// Called every frame
void AControlPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BPReadDate();

	if(ControlledLight)
	{
		ControlledLight->SetActorRotation(FMath::RInterpTo(ControlledLight->GetActorRotation(), TargetLightRotation, DeltaTime, LerpRate));	
	}
	if(RotateControlledActor)
	{
		RotateControlledActor->SetActorRotation(FMath::RInterpTo(RotateControlledActor->GetActorRotation(), TargetLevelRotation, DeltaTime, LerpRate));
	}
}

// Called to bind functionality to input
void AControlPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputComp) {
		InputComp->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AControlPawn::RotateLightWithEnhancedInput);
		InputComp->BindAction(SwitchAction, ETriggerEvent::Started, this, &AControlPawn::SwitchControlMethod);
		InputComp->BindAction(ChangeLightAction, ETriggerEvent::Started, this, &AControlPawn::ChangeLightWithEnhancedInput);
		InputComp->BindAction(ChangeMapAction, ETriggerEvent::Started, this, &AControlPawn::ChangeMapWithEnhancedInput);
		InputComp->BindAction(ChangeMirrorAction, ETriggerEvent::Started, this, &AControlPawn::ShowMirrorWithEnhancedInput);
		InputComp->BindAction(ChangeMirrorAction, ETriggerEvent::Completed, this, &AControlPawn::HideMirrorWithEnhancedInput);
		InputComp->BindAction(RotateWithMouseAction, ETriggerEvent::Triggered, this, &AControlPawn::RotateWithMouseInput);
		InputComp->BindAction(RotateLevelAction, ETriggerEvent::Started, this, &AControlPawn::RotateLevelWithEnhancedInput);
	}
}

void AControlPawn::RotateLightWithEnhancedInput(const FInputActionValue& Value) {
	if(UseHardware) return;
	
	FVector2D Input = Value.Get<FVector2D>();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Rotate: %f, %f"), Input.X, Input.Y));

	if (ControlledLight) {
		//Calculate the Angle
		FVector2D Base(0, -1);
		double Yaw = FMath::Atan2(Input.Y, Input.X) - FMath::Atan2(Base.Y, Base.X);
		Yaw = FMath::RadiansToDegrees(Yaw);
		if (Yaw <= 0) Yaw = FMath::Abs(Yaw);
		else Yaw = 360.0f - Yaw;

		//Calculate thr Height Of Sun. (SunHeight == 0: Highest, SunHeight == 1: Lowest)
		double SunHeight = FMath::Sqrt(FMath::Square(Input.X) + FMath::Square(Input.Y));
		SunHeight = FMath::Min(SunHeight, 1.0f);
		double YRotation;
		//if (Input.Y >= 0) 
		YRotation = 270 + SunHeight * 90;
		//else YRotation = 270 + SunHeight * 90;
		//FRotator CurRotator = ControledLight->GetActorRotation();
		FRotator rotator(YRotation, Yaw, 0);
		TargetLightRotation = rotator;
		//r =  FQuat::Slerp(CurRotator.Quaternion(), rotator.Quaternion(), LerpRate).Rotator();
		//ControledLight->SetActorRotation(rotator);
	}
}

//Whether use hardware, hardware control is opened in default
void AControlPawn::SwitchControlMethod(){
	UseHardware = !UseHardware;
	SunbeamGameInstance->HardwareControlEnabled = UseHardware;
	if (UseHardware) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hardware control open")));
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hardware control close")));
}

void AControlPawn::ChangeLightWithEnhancedInput(const FInputActionValue& Value) {
	if(UseHardware) return;
	
	float Input = Value.Get<float>();
	int32 Input_int = FMath::RoundToInt32(Input)-1;
	if (EnabledLightIndex == Input_int) return;
	ChangeLight(Input_int);
}

void AControlPawn::ChangeMapWithEnhancedInput(const FInputActionValue& Value)
{
	if(UseHardware) return;
	
	float Input = Value.Get<float>();
	int32 Input_int = FMath::RoundToInt32(Input);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Map Index: %d"), Input_int));
	ChangeMap(Input_int-1);
}

void AControlPawn::ShowMirrorWithEnhancedInput(const FInputActionValue& Value)
{
	if(UseHardware) return;
	
	//if using NightLight, do not show focal
	if(EnabledLightIndex == 1) return;
	float Input = Value.Get<float>();
	int32 Input_int = FMath::RoundToInt32(Input) - 1;
	MirrorIndex = Input_int;
	ChangeMirror(true);
}

void AControlPawn::HideMirrorWithEnhancedInput(const FInputActionValue& Value)
{
	//if using NightLight, do not show focal
	if(EnabledLightIndex == 1) return;
	ChangeMirror(false);
}

void AControlPawn::RotateWithMouseInput(const FInputActionValue& Value)
{
	if (UseHardware)
	{
		return;
	}

	// Update target rotator
	FVector2D Input = Value.Get<FVector2D>();
	FRotator NewRotator = TargetLightRotation;
	NewRotator.Yaw += Input.X * MouseSensitivity;
	NewRotator.Pitch += Input.Y * MouseSensitivity;
	TargetLightRotation = NewRotator;
}

void AControlPawn::RotateLevelWithEnhancedInput(const FInputActionValue& Value)
{
	if(UseHardware) return;
	
	float Input = Value.Get<float>();

	if(RotateControlledActor)
	{
		FRotator Rotation = RotateControlledActor->GetActorRotation();
		//ture right, negative
		if(Input > 0)
		{
			if(RotateIndex >= 0 || FMath::Abs(RotateIndex) < MaxRotateIndex)
			{
				RotateIndex--;
				Rotation.Yaw -= RotatePerAngle;
				TargetLevelRotation = Rotation;
			}
		}
		//ture left, positive
		else
		{
			if(RotateIndex <= 0 || RotateIndex < MaxRotateIndex)
			{
				RotateIndex++;
				Rotation.Yaw += RotatePerAngle;
				TargetLevelRotation = Rotation;
			}
		}
	}
}

void AControlPawn::ChangeLightWithHardware(int index){
	if (EnabledLightIndex == index) return;
	ChangeLight(index);
}

void AControlPawn::ChangeMapWithHardware(int index)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Map Index: %d"), index));
	ChangeMap(index);
}

void AControlPawn::ChangeMirrorWithHardware(int index)
{
	if(MirrorIndex == index) return;
	MirrorIndex = index;
	GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Blue, FString::Printf(TEXT("Mirror Index: %d"), index));
	ChangeMirror(index != 0);
}

void AControlPawn::RotateLevelWithHardware(int index)
{
	
}

void AControlPawn::LightsInitialize()
{
	UClass* DirectionalLightClass = ADirectionalLight::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), DirectionalLightClass, TEXT("SunLight"), OutActors);
	if(OutActors.Num() != 0)
	{
		SunLight = Cast<ADirectionalLight>(OutActors[0]);
		Lights.Emplace(SunLight);
		ControlledLight = SunLight;
		TargetLightRotation = ControlledLight->GetActorRotation();
	}

	OutActors.Empty();

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), DirectionalLightClass, TEXT("MoonLight"), OutActors);
	if(OutActors.Num() != 0)
	{
		MoonLight = Cast<ADirectionalLight>(OutActors[0]);
		MoonLight->SetActorHiddenInGame(true);
		Lights.Emplace(MoonLight);
	}
}

void AControlPawn::ControlledActorsInitialize()
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("RotateRoot"), OutActors);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("DayItem"), DayItems);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("NightItem"), NightItems);

	if(OutActors.Num() == 1)
	{
		RotateControlledActor = OutActors[0];
		TargetLevelRotation = RotateControlledActor->GetActorRotation();
	}
	for(int i = 0; i<NightItems.Num(); i++){
		NightItems[i]->SetActorHiddenInGame(true);
		NightItems[i]->SetActorEnableCollision(false);
	}
}

void AControlPawn::ChangeLight(int index)
{
	//Enable SunLight and DayItems
	if(index == 0)
	{
		EnabledLightIndex = 0;
		ControlledLight = Lights[index];
		if(SunLight && MoonLight)
		{
			SunLight->SetActorHiddenInGame(false);
			MoonLight->SetActorHiddenInGame(true);
			SunLight->SetActorRotation(MoonLight->GetActorRotation());
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, FString::Printf(TEXT("Missing SunLight or MoonLight in the scene")));
		}
		ChangeLightBeam();
	}
	//Enable MoonLight and NightItems
	else
	{
		EnabledLightIndex = 1;
		ControlledLight = Lights[index];
		if(SunLight && MoonLight)
		{
			SunLight->SetActorHiddenInGame(true);
			MoonLight->SetActorHiddenInGame(false);
			MoonLight->SetActorRotation(SunLight->GetActorRotation());
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(10, 2.f, FColor::Red, FString::Printf(TEXT("Missing SunLight or MoonLight in the scene")));
		}
		ChangeLightBeam();
		for(int i = 0; i<DayItems.Num(); i++){
			DayItems[i]->SetActorHiddenInGame(true);
			DayItems[i]->SetActorEnableCollision(false);
		}
		for(int i = 0; i<NightItems.Num(); i++){
			NightItems[i]->SetActorHiddenInGame(false);
			NightItems[i]->SetActorEnableCollision(true);
		}
	}
}

void AControlPawn::ChangeMap(int index)
{
	if(UGameInstance* GameIns = GetGameInstance())
	{
		Cast<USunbeamGameInstance>(GameIns)->ChangeMap(index);
	}
}
