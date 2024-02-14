// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct FBeamGameplayTags
{
	static const FBeamGameplayTags& Get()
	{
		return GameplayTags;
	}

	static void InitializeGameplayTags();

	FGameplayTag LightSource_Sun;
	FGameplayTag LightSource_Moon;
    
private:
    static FBeamGameplayTags GameplayTags;
};