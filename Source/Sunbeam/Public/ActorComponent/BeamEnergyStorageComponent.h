// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "BeamEnergyStorageComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeamEnergyChanged, int32, NewCurEnergy);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUNBEAM_API UBeamEnergyStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBeamEnergyStorageComponent();

	bool ConsumeEnergy(int32 EnergyToConsume, FGameplayTag EnergyTypeTag);

	bool AddEnergy(int32 EnergyToAdd, FGameplayTag EnergyTypeTag);
	
	int32 GetCurEnergy(FGameplayTag EnergyTypeTag) const;

	int32 GetMaxEnergy() const;

	FOnBeamEnergyChanged OnBeamEnergyChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy")
	TMap<FGameplayTag, int32> CurrentEnergyPerTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Energy")
	int32 MaxEnergy = 10;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
