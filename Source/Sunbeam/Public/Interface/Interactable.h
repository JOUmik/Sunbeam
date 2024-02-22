// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

class ABeamActor;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUNBEAM_API IInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnBeginInteract(FHitResult BeamHitResult, const ABeamActor* BeamActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnEndInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnTickInteract(FHitResult BeamHitResult, const ABeamActor* BeamActor, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void GetInteractableTags(FGameplayTagContainer& OutTagContainer);
};
