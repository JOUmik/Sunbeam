// Copyright Project SunBeam. All Rights Reserved.

#include "ActorComponent/BeamEnergyStorageComponent.h"

#include "GameplayTagContainer.h"

// Sets default values for this component's properties
UBeamEnergyStorageComponent::UBeamEnergyStorageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UBeamEnergyStorageComponent::ConsumeEnergy(int32 EnergyToConsume, const FGameplayTag EnergyTypeTag)
{
	int32 CurEnergy = CurrentEnergyPerTag.FindChecked(EnergyTypeTag);
	if (EnergyToConsume < 0 || EnergyToConsume > CurEnergy)
	{
		return false;
	}

	CurEnergy -= EnergyToConsume;
	CurrentEnergyPerTag.Add(EnergyTypeTag, CurEnergy);
	OnBeamEnergyChangedDelegate.Broadcast(CurEnergy);
	
	return true;
}

bool UBeamEnergyStorageComponent::AddEnergy(int32 EnergyToAdd, const FGameplayTag EnergyTypeTag)
{
	if (!CurrentEnergyPerTag.Contains(EnergyTypeTag))
	{
		CurrentEnergyPerTag.Add(EnergyTypeTag, 0);
	}
	
	int32 CurEnergy = CurrentEnergyPerTag.FindChecked(EnergyTypeTag);
	if (EnergyToAdd < 0 || CurEnergy + EnergyToAdd > MaxEnergy)
	{
		return false;
	}

	CurEnergy += EnergyToAdd;
	CurrentEnergyPerTag.Add(EnergyTypeTag, CurEnergy);
	OnBeamEnergyChangedDelegate.Broadcast(CurEnergy);
	
	return true;
}

int32 UBeamEnergyStorageComponent::GetCurEnergy(const FGameplayTag EnergyTypeTag) const
{
	if (!CurrentEnergyPerTag.Contains(EnergyTypeTag))
	{
		return 0;
	}
	
	return CurrentEnergyPerTag.FindChecked(EnergyTypeTag);
}

int32 UBeamEnergyStorageComponent::GetMaxEnergy() const
{
	return MaxEnergy;
}

// Called when the game starts
void UBeamEnergyStorageComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

