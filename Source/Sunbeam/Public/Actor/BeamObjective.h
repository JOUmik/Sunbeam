// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "BeamObjective.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveStateChanged, bool, NewState);

UCLASS()
class SUNBEAM_API ABeamObjective : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeamObjective();

	/* IInteractable interface */
	virtual void OnBeginInteract_Implementation(FHitResult LightHitResult, AActor* LightSource) override;
	virtual void OnEndInteract_Implementation() override;
	virtual void GetInteractableResponseTags_Implementation(FGameplayTagContainer& OutTagContainer) override;
	virtual void GetInteractableAssetTag_Implementation(FGameplayTag& OutGameplayTag) override;
	/* End IInteractable interface */

	UFUNCTION(BlueprintImplementableEvent, Category = "Objective")
	void PlayBloomAnimForward();

	UFUNCTION(BlueprintImplementableEvent, Category = "Objective")
	void PlayBloomAnimReverse();

	UFUNCTION(BlueprintCallable, Category = "Objective")
	void SetBloomStatus(bool bBloomed);

	UPROPERTY(BlueprintAssignable, Category = "Objective")
	FOnObjectiveStateChanged OnObjectiveStateChangedDelegate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
	FGameplayTagContainer InteractableResponseTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Objective")
	FGameplayTag InteractableAssetTag;

private:
	int32 LightSourceCount = 0;

};
