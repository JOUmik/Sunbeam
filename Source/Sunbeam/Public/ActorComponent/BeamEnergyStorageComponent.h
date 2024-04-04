// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "BeamEnergyStorageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeamEnergyChanged, int32, NewEnergy, FGameplayTag, EnergyTypeTag);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUNBEAM_API UBeamEnergyStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBeamEnergyStorageComponent();

	bool ConsumeEnergy(int32 EnergyToConsume, FGameplayTag EnergyTypeTag);

	bool AddEnergy(int32 EnergyToAdd, FGameplayTag EnergyTypeTag);

	FOnBeamEnergyChanged OnBeamEnergyChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy")
	TMap<FGameplayTag, int32> CurrentEnergyPerTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy")
	int32 MaxEnergy = 10;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE int32 GetCurEnergy(const FGameplayTag EnergyTypeTag) const
	{
		// For interactables, initially the energy type tag will be empty
		if (!CurrentEnergyPerTag.Contains(EnergyTypeTag))
		{
			return 0;
		}
	
		return CurrentEnergyPerTag.FindChecked(EnergyTypeTag);
	}
	
	FORCEINLINE int32 GetMaxEnergy() const { return MaxEnergy; }
	FORCEINLINE TMap<FGameplayTag, int32> GetCurrentEnergyPerTag() const { return CurrentEnergyPerTag; }
};
