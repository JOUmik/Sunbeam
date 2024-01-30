// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
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
	ItemsInitialize();

}

// Called every frame
void AControlPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BPReadDate();

	ControledLight->SetActorRotation(FMath::RInterpTo(ControledLight->GetActorRotation(), TargetRotator, UGameplayStatics::GetWorldDeltaSeconds(this), LerpRate));
}

// Called to bind functionality to input
void AControlPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputComp) {
		InputComp->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AControlPawn::RotateWithEnhancedInput);
		InputComp->BindAction(SwitchAction, ETriggerEvent::Started, this, &AControlPawn::Switch);
		InputComp->BindAction(HardwareSelectAction, ETriggerEvent::Started, this, &AControlPawn::HardwareSelect);
		InputComp->BindAction(ChangeLightAction, ETriggerEvent::Started, this, &AControlPawn::ChangeLightWithEnhancedInput);
		InputComp->BindAction(ChangeMapAction, ETriggerEvent::Started, this, &AControlPawn::ChangeMapWithEnhancedInput);
		InputComp->BindAction(ChangeMirrorAction, ETriggerEvent::Started, this, &AControlPawn::ChangeMirrorWithEnhancedInput);
		InputComp->BindAction(ChangeMirrorAction, ETriggerEvent::Completed, this, &AControlPawn::ChangeMirrorWithEnhancedInput);
	}
}

void AControlPawn::RotateWithEnhancedInput(const FInputActionValue& Value) {
	FVector2D Input = Value.Get<FVector2D>();
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Rotate: %f, %f"), Input.X, Input.Y));

	if (ControledLight) {
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
		TargetRotator = rotator;
		//r =  FQuat::Slerp(CurRotator.Quaternion(), rotator.Quaternion(), LerpRate).Rotator();
		//ControledLight->SetActorRotation(rotator);
	}
}

void AControlPawn::RotateWithHardware_JoyCon() {
	//if _X > 1.f, it means can no get input from hardware
	if (_X > 1.f) return;
	
	
	if (ControledLight) {
		//Calculate the Angle
		FVector2D Base(0, -1);
		double Yaw = FMath::Atan2(_Y, _X) - FMath::Atan2(Base.Y, Base.X);
		Yaw = FMath::RadiansToDegrees(Yaw);
		if (Yaw <= 0) Yaw = FMath::Abs(Yaw);
		//else if (Yaw <= 90) Yaw = 360.0f - Yaw;
		else Yaw = 360.0f - Yaw;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Rotate Angle: %f"), Yaw));

		//Calculate thr Height Of Sun. (SunHeight == 0: Highest, SunHeight == 1: Lowest)
		double SunHeight = FMath::Sqrt(FMath::Square(_X) + FMath::Square(_Y));
		SunHeight = FMath::Min(SunHeight, 1.0f);
		double YRotation;
		//if (Input.Y >= 0) 
		YRotation = 270 + SunHeight * 90;
		//else YRotation = 270 + SunHeight * 90;
		FRotator rotator(YRotation, Yaw, 0);
		TargetRotator = rotator;
		//ControledLight->SetActorRotation(FMath::RInterpTo(GetActorRotation(), rotator, UGameplayStatics::GetWorldDeltaSeconds(this), LerpRate));
	}
	
	
}

void AControlPawn::RotateWithHardware_Gyro() {


	//if _Yaw < -500.f, it means can no get input from hardware
	if (_Yaw < -500.f) return;

	FRotator CurRotation = ControledLight->GetActorRotation();
	CurRotation.Pitch = _Pitch;
	CurRotation.Yaw = _Yaw;
	CurRotation.Roll = _Roll;
	
	ControledLight->SetActorRotation(FMath::RInterpTo(GetActorRotation(), CurRotation, UGameplayStatics::GetWorldDeltaSeconds(this), LerpRate));
}

void AControlPawn::Switch(){
	UseHardware = !UseHardware;
	if (UseHardware) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hardware control open")));
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hardware control close")));
}

void AControlPawn::HardwareSelect(const FInputActionValue& Value) {
	float Input = Value.Get<float>();

	int32 Input_int = FMath::RoundToInt32(Input);

	SelectedHardware = Input_int;
	if (Input_int == 1) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Use JoyCon Hardware")));
	}
	else if (Input_int == 2) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Use Gyro Hardware")));
	}
}

void AControlPawn::ChangeLightWithEnhancedInput(const FInputActionValue& Value) {
	float Input = Value.Get<float>();
	int32 Input_int = FMath::RoundToInt32(Input)-1;
	if (EnabledLightIndex == Input_int) return;
	ChangeLight(Input_int);
}

void AControlPawn::ChangeMapWithEnhancedInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();
	int32 Input_int = FMath::RoundToInt32(Input);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Map Index: %d"), Input_int));
	ChangeMap(Input_int);
}

void AControlPawn::ChangeMirrorWithEnhancedInput(const FInputActionValue& Value)
{
	float Input = Value.Get<float>();
	int32 Input_int = FMath::RoundToInt32(Input) - 1;
	MirrorIndex = Input_int;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Mirror Index: %d"), Input_int));
	ChangeMirror();
}

void AControlPawn::ChangeLightWithHardware(int index){
	if (EnabledLightIndex == index) return;
	ChangeLight(index);
}

void AControlPawn::ChangeMapWithHardware(int index)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Map Index: %d"), index));
	ChangeMap(index);
}

void AControlPawn::ChangeMirrorWithHardware(int index)
{
	if(MirrorIndex == index) return;
	MirrorIndex = index;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Mirror Index: %d"), index));
	ChangeMirror();
}

void AControlPawn::LightsInitialize()
{
	UClass* DirectionalLightClass = ADirectionalLight::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), DirectionalLightClass, TEXT("SunLight"), OutActors);
	SunLight = Cast<ADirectionalLight>(OutActors[0]);
	Lights.Emplace(SunLight);
	ControledLight = SunLight;
	TargetRotator = ControledLight->GetActorRotation();

	OutActors.Empty();

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), DirectionalLightClass, TEXT("MoonLight"), OutActors);
	MoonLight = Cast<ADirectionalLight>(OutActors[0]);
	MoonLight->SetActorHiddenInGame(true);
	Lights.Emplace(MoonLight);
}

void AControlPawn::ItemsInitialize()
{
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), TEXT("Focal"), Focals);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStaticMeshActor::StaticClass(), TEXT("DayItem"), DayItems);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AStaticMeshActor::StaticClass(), TEXT("NightItem"), NightItems);

	for(int i = 0; i<Focals.Num(); i++)
	{
		Focals[i]->SetActorHiddenInGame(true);
	}

	for(int i = 0; i<NightItems.Num(); i++){
		NightItems[i]->SetActorHiddenInGame(true);
	}
}

void AControlPawn::ChangeLight(int index)
{
	//Enable SunLight and DayItems
	if(index == 0)
	{
		EnabledLightIndex = 0;
		ControledLight = Lights[index];
		SunLight->SetActorHiddenInGame(false);
		MoonLight->SetActorHiddenInGame(true);
		for(int i = 0; i<DayItems.Num(); i++){
			DayItems[i]->SetActorHiddenInGame(false);
		}
		for(int i = 0; i<NightItems.Num(); i++){
			NightItems[i]->SetActorHiddenInGame(true);
		}
	}
	//Enable MoonLight and NightItems
	else
	{
		EnabledLightIndex = 1;
		ControledLight = Lights[index];
		SunLight->SetActorHiddenInGame(true);
		MoonLight->SetActorHiddenInGame(false);
		for(int i = 0; i<DayItems.Num(); i++){
			DayItems[i]->SetActorHiddenInGame(true);
		}
		for(int i = 0; i<NightItems.Num(); i++){
			NightItems[i]->SetActorHiddenInGame(false);
		}
	}
}

void AControlPawn::ChangeMap(int index)
{
}

void AControlPawn::ChangeMirror()
{
	//if using NightLight, do not show focal
	if(EnabledLightIndex == 1) return;
	
	for(int i = 0; i<Focals.Num(); i++)
	{
		Focals[i]->SetActorHiddenInGame(!Focals[i]->IsHidden());
	}
}
