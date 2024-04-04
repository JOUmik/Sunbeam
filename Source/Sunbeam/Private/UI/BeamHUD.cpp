// Copyright Project SunBeam. All Rights Reserved.


#include "UI/BeamHUD.h"

#include "UI/BeamUserWidget.h"
#include "UI/OverlayWidgetController.h"

UOverlayWidgetController* ABeamHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

void ABeamHUD::InitOverlay(UBeamEnergyStorageComponent* InEnergyStorageComponent, ABeamGameModeBase* InGameMode)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_BeamHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_BeamHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UBeamUserWidget>(Widget);

	const FWidgetControllerParams WCParams(InGameMode, InEnergyStorageComponent);
	OverlayWidgetController = GetOverlayWidgetController(WCParams);
	OverlayWidget->SetWidgetController(OverlayWidgetController);

	OverlayWidgetController->BroadCastInitialValues();

	OverlayWidget->AddToViewport();
}
