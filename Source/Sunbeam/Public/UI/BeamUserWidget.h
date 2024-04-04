// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BeamUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUNBEAM_API UBeamUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Widget")
	void OnWidgetControllerSet();
};
