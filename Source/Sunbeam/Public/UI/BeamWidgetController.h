// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BeamWidgetController.generated.h"

class ABeamGameModeBase;
class UBeamEnergyStorageComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(ABeamGameModeBase* InBeamGameMode, UBeamEnergyStorageComponent* InEnergyStorageComponent)
		: BeamGameMode(InBeamGameMode), EnergyStorageComponent(InEnergyStorageComponent) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<ABeamGameModeBase> BeamGameMode = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBeamEnergyStorageComponent> EnergyStorageComponent = nullptr;
};

/**
 * 
 */
UCLASS()
class SUNBEAM_API UBeamWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams);

	virtual void BroadCastInitialValues() {}
	virtual void BindCallbacksToDependencies() {}

protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UBeamEnergyStorageComponent> EnergyStorageComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<ABeamGameModeBase> BeamGameMode;
};
