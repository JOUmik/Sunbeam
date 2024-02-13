// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
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
	void OnBeginInteract(FHitResult BeamHitResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnEndInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	void OnTickInteract(FHitResult BeamHitResult, float DeltaTime);
};
