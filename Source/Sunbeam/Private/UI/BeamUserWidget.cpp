// Copyright Project SunBeam. All Rights Reserved.


#include "UI/BeamUserWidget.h"

void UBeamUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
