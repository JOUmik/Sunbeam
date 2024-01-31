// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ControlPawn.generated.h"

class USunbeamGameInstance;
class UInputMappingContext;
class UInputComponent;
class UInputAction;
struct FInputActionValue;
class ADirectionalLight;


UCLASS()
class SUNBEAM_API AControlPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AControlPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _X = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _Y = 0; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _Pitch = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _Yaw = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _Roll = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseHardware = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectedHardware = 1;         //1:JoyCon, 2:Gyro

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting")
	UInputMappingContext* MappingContext;

	UFUNCTION(BlueprintImplementableEvent, Category = "CPPSettings|CatchBlueprintFunction")
	void BPReadDate();

	UPROPERTY(BlueprintReadOnly)
	ADirectionalLight* ControledLight;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeMirror(bool bShow);
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeLightBeam();

	UPROPERTY(BlueprintReadOnly)
	int EnabledLightIndex = 0;

private:
	//Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* HardwareSelectAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeLightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeMapAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeMirrorAction;

	void RotateWithEnhancedInput(const FInputActionValue& Value);
	void Switch();
	void HardwareSelect(const FInputActionValue& Value);
	void ChangeLightWithEnhancedInput(const FInputActionValue& Value);
	void ChangeMapWithEnhancedInput(const FInputActionValue& Value);
	void ShowMirrorWithEnhancedInput(const FInputActionValue& Value);
	void HideMirrorWithEnhancedInput(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void RotateWithHardware_JoyCon();
	UFUNCTION(BlueprintCallable)
	void RotateWithHardware_Gyro();
	UFUNCTION(BlueprintCallable)
	void ChangeLightWithHardware(int index);
	UFUNCTION(BlueprintCallable)
	void ChangeMapWithHardware(int index);
	UFUNCTION(BlueprintCallable)
	void ChangeMirrorWithHardware(int index);

	void LightsInitialize();
	void ItemsInitialize();

	void ChangeLight(int index);
	void ChangeMap(int index);
	
	ADirectionalLight* SunLight;
	ADirectionalLight* MoonLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ADirectionalLight*> Lights;

	//TArray<AActor*> Focals;
	TArray<AActor*> DayItems;
	TArray<AActor*> NightItems;

	int MapIndex = 0;
	int MirrorIndex = 0;

	FTimerHandle RotateTimeHandle;

	UPROPERTY(EditAnywhere, Category = "CPPSettings|Lerp Setting")
	double LerpRate = 10.f;

	FRotator CurRotator, TargetRotator;
	USunbeamGameInstance* SunbeamGameInstance;
};
