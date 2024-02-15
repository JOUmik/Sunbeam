// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "BeamFlowerBase.generated.h"

UCLASS()
class SUNBEAM_API ABeamFlowerBase : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeamFlowerBase();

	virtual void GetInteractableTags_Implementation(FGameplayTagContainer& OutTagContainer) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/**
	 * @brief Call this from blueprint to set the bloom status of the flower.
	 *
	 * Add energy to the player if the flower is not in bloom, subtract energy from the player if the flower is in bloom.
	 * 
	 * @param bBloomed The bloom status of the flower. True if bloom animation reaches the end, False if bloom animation was played in reverse and reaches the start.
	 */
	UFUNCTION(BlueprintCallable, Category = "Flower")
	void SetBloomStatus(bool bBloomed);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flower")
	int32 EnergyValue = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flower")
	FGameplayTagContainer InteractableTags;

private:
	bool bHasBloomed = false;
};
