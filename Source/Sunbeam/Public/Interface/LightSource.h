// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "LightSource.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULightSource : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUNBEAM_API ILightSource
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	FGameplayTag GetLightSourceTag() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interactable")
	bool CanInteractWithActor(AActor* OtherActor) const;
};
