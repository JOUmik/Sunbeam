// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LightSource.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

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
	void OnBeginInteract(FHitResult LightHitResult, AActor* LightSource);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnEndInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnTickInteract(FHitResult LightHitResult, AActor* LightSource, float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void GetInteractableResponseTags(FGameplayTagContainer& OutTagContainer);
};
