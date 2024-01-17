// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"

#define _USE_MATH_DEFINES
#include <cmath>

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

	UClass* DirectionalLightClass = ADirectionalLight::StaticClass();
	SunLight = Cast<ADirectionalLight>(UGameplayStatics::GetActorOfClass(GetWorld(), DirectionalLightClass));
}

// Called every frame
void AControlPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AControlPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputComp) {
		InputComp->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AControlPawn::Rotate);
	}
}

void AControlPawn::Rotate(const FInputActionValue& Value) {
	FVector2D Input = Value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Rotate: %f, %f"), Input.X, Input.Y));

	if (SunLight) {
		//Calculate the Angle
		FVector2D Base(0, -1);
		double Yaw = FMath::Atan2(Input.Y, Input.X) - FMath::Atan2(Base.Y, Base.X);
		Yaw = FMath::RadiansToDegrees(Yaw);
		if (Yaw <= 0) Yaw = FMath::Abs(Yaw);
		//else if (Yaw <= 90) Yaw = 360.0f - Yaw;
		else Yaw = 360.0f - Yaw;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Rotate Angle: %f"), Yaw));

		//Calculate thr Height Of Sun. (SunHeight == 0: Highest, SunHeight == 1: Lowest)
		double SunHeight = FMath::Sqrt(FMath::Square(Input.X) + FMath::Square(Input.Y));
		SunHeight = FMath::Min(SunHeight, 1.0f);
		double YRotation;
		//if (Input.Y >= 0) 
			YRotation = 270 + SunHeight * 90;
		//else YRotation = 270 + SunHeight * 90;
		FRotator rotator(YRotation, Yaw, 0);

		SunLight->SetActorRotation(rotator);
	}
}