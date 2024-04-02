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

	FGameplayTag LightSourceBase;
	FGameplayTag LightSource_Sun;
	FGameplayTag LightSource_Moon;

	FGameplayTag SecondarySourceBase;
	FGameplayTag SecondarySource_Sunflower;
	FGameplayTag SecondarySource_Lamp;
    
private:
    static FBeamGameplayTags GameplayTags;
};