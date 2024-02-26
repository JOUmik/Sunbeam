// Copyright Project SunBeam. All Rights Reserved.

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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting")
	UInputMappingContext* MappingContext;

	UFUNCTION(BlueprintImplementableEvent, Category = "CPPSettings|CatchBlueprintFunction")
	void BPReadDate();

	UPROPERTY(BlueprintReadOnly)
	ADirectionalLight* ControlledLight;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeMirror(bool bShow);
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeLightBeam();

	UPROPERTY(BlueprintReadOnly)
	int EnabledLightIndex = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseHardware = true;

	/*
	 * RotateLevel Parameters
	 */
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CPPSettings|Input Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* RotateAction;
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
	void RotateLightWithEnhancedInput(const FInputActionValue& Value);
	void SwitchControlMethod();
	void ChangeLightWithEnhancedInput(const FInputActionValue& Value);
	void ChangeMapWithEnhancedInput(const FInputActionValue& Value);
	void ShowMirrorWithEnhancedInput(const FInputActionValue& Value);
	void HideMirrorWithEnhancedInput(const FInputActionValue& Value);
	void RotateWithMouseInput(const FInputActionValue& Value);
	void RotateLevelWithEnhancedInput(const FInputActionValue& Value);

	/*
	 * Hardware control functions
	 */
	UFUNCTION(BlueprintCallable)
	void ChangeLightWithHardware(int index);
	UFUNCTION(BlueprintCallable)
	void ChangeMapWithHardware(int index);
	UFUNCTION(BlueprintCallable)
	void ChangeMirrorWithHardware(int index);
	UFUNCTION(BlueprintCallable)
	void RotateLevelWithHardware(int index);

	/*
	 * Initialize in BeginPlay
	 */
	void LightsInitialize();
	void ControlledActorsInitialize();

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

	FRotator CurRotator, TargetLightRotation;
	USunbeamGameInstance* SunbeamGameInstance;
	
};
