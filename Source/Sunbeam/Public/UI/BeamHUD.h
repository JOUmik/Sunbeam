// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BeamHUD.generated.h"

class UBeamUserWidget;
class ABeamGameModeBase;
class UBeamEnergyStorageComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class SUNBEAM_API ABeamHUD : public AHUD
{
	GENERATED_BODY()

public:
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(UBeamEnergyStorageComponent* InEnergyStorageComponent, ABeamGameModeBase* InGameMode);

private:
	UPROPERTY()
	TObjectPtr<UBeamUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBeamUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
