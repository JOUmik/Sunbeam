// Copyright Project SunBeam. All Rights Reserved.


#include "Singleton/BeamGameplayTags.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

FBeamGameplayTags FBeamGameplayTags::GameplayTags;

void FBeamGameplayTags::InitializeGameplayTags()
{
	GameplayTags.LightSource_Sun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("LightSource.Sun"),
		FString("Gameplay Tag for Sun Light Source")
		);

	GameplayTags.LightSource_Moon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("LightSource.Moon"),
		FString("Gameplay Tag for Moon Light Source")
		);

	GameplayTags.SecondarySource_Sunflower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SecondarySource.Sunflower"),
		FString("Gameplay Tag for Sunflower Secondary Light Source")
		);
}
