// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BeamPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class ADirectionalLight;
class USunbeamGameInstance;

/**
 * 
 */
UCLASS()
class SUNBEAM_API ABeamPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "CPPSettings|CatchBlueprintFunction")
	void BPReadDate();

	UPROPERTY(BlueprintReadOnly)
	ADirectionalLight* ControlledLight;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TArray<FName> LevelToLoad;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeMirror(bool bShow);
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeLightBeam();

	UPROPERTY(BlueprintReadOnly)
	int EnabledLightIndex = 0;

protected:
	virtual void BeginPlay() override;
	
	void RotateBeamWithMouseInput(const FInputActionValue& InputActionValue);
	void RotateBeamWithJoystick(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable)
	void SwitchBeamState(int32 index);
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseHardware = true;

	/*
	 * RotateLevel Parameters
	 */
	UPROPERTY()
	AActor* RotateControlledActor;
	FRotator TargetLevelRotation;
	int32 OriginalIndex = 0;  //For Hardware Control
	UPROPERTY(VisibleAnywhere, Category = "CPP Settings|Rotate Level")
	int32 RotateIndex = 0;
	UPROPERTY(EditAnywhere, Category = "CPP Settings|Rotate Level")
	float RotatePerAngle = 5.f;
	UPROPERTY(EditAnywhere, Category = "CPP Settings|Rotate Level")
	int32 MaxRotateIndex = 6;

private:
	/*
	* EnhancedInput Actions
	*/
	// Additively apply 2D Axis input to the beam rotation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateWithMouseInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RotateWithJoystickAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeLightAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeMapAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* ChangeMirrorAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateWithMouseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateLevelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	float MouseSensitivity = 0.1f;

	/*
	 * EnhanceInput control functions
	 */
	void SwitchControlMethod();
	void ChangeMapWithEnhancedInput(const FInputActionValue& Value);
	void ShowMirrorWithEnhancedInput(const FInputActionValue& Value);
	void HideMirrorWithEnhancedInput(const FInputActionValue& Value);
	void RotateLevelWithEnhancedInput(const FInputActionValue& Value);

	/*
	 * Hardware control functions
	 */
	UFUNCTION(BlueprintCallable)
	void ChangeMapWithHardware(int index);
	UFUNCTION(BlueprintCallable)
	void ChangeMirrorWithHardware(int index);
	UFUNCTION(BlueprintCallable)
	void RotateLevelWithHardware(int index);
	
	void ChangeMap(int index);
	void RotateLevel(const int input);

	int MapIndex = 0;
	int MirrorIndex = 0;

	FTimerHandle RotateTimeHandle;

	UPROPERTY(EditAnywhere, Category = "CPPSettings|Lerp Setting")
		double LerpRate = 10.f;

	FRotator CurRotator, TargetLightRotation;
	USunbeamGameInstance* SunbeamGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SwitchBeamState1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> SwitchBeamState2Action;
};
