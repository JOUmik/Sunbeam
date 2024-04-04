// Copyright Project SunBeam. All Rights Reserved.


#include "UI/BeamWidgetController.h"

void UBeamWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InWidgetControllerParams)
{
	EnergyStorageComponent = InWidgetControllerParams.EnergyStorageComponent;
	BeamGameMode = InWidgetControllerParams.BeamGameMode;
}
