// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/BeamWidgetController.h"
#include "OverlayWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveCountChanged, int32, NewCompletedCount, int32, NewObjectiveCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerBeamEnergyChanged, float, NewEnergy, FGameplayTag, EnergyType);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Sunbeam))
class SUNBEAM_API UOverlayWidgetController : public UBeamWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadCastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION()
	void OnEnergyChanged(int32 NewEnergy, FGameplayTag EnergyType);

	UFUNCTION()
	void OnObjectiveCompletedCountChanged(const int32 NewCount, const int32 MaxCount);

	UPROPERTY(BlueprintAssignable, Category = "Sunbeam|Attributes")
	FOnObjectiveCountChanged OnObjectiveCountChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Sunbeam|Attributes")
	FOnPlayerBeamEnergyChanged OnBeamEnergyChangedDelegate;
};
