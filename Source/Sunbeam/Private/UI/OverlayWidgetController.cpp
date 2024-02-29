// Copyright Project SunBeam. All Rights Reserved.


#include "UI/OverlayWidgetController.h"

#include "ActorComponent/BeamEnergyStorageComponent.h"
#include "Game/BeamGameModeBase.h"

void UOverlayWidgetController::BroadCastInitialValues()
{
	Super::BroadCastInitialValues();

	const int32 ObjectiveCompletedCount = BeamGameMode->GetObjectiveCompletedCount();
	const int32 ObjectiveMaxCount = BeamGameMode->GetObjectiveCount();
	
	OnObjectiveCountChangedDelegate.Broadcast(ObjectiveCompletedCount, ObjectiveMaxCount);

	TMap<FGameplayTag, int32> EnergyPerTag = EnergyStorageComponent->GetCurrentEnergyPerTag();
	for (const auto& Elem : EnergyPerTag)
	{
		OnBeamEnergyChangedDelegate.Broadcast(Elem.Value, Elem.Key);
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();

	EnergyStorageComponent->OnBeamEnergyChangedDelegate.AddDynamic(this, &UOverlayWidgetController::OnEnergyChanged);
	BeamGameMode->OnObjectiveCompletedCountChangeDelegate.AddDynamic(this, &UOverlayWidgetController::OnObjectiveCompletedCountChanged);
}

void UOverlayWidgetController::OnEnergyChanged(const int32 NewEnergy, const FGameplayTag EnergyType)
{
	OnBeamEnergyChangedDelegate.Broadcast(NewEnergy, EnergyType);
}

void UOverlayWidgetController::OnObjectiveCompletedCountChanged(const int32 NewCount, const int32 MaxCount)
{
	OnObjectiveCountChangedDelegate.Broadcast(NewCount, MaxCount);
}
