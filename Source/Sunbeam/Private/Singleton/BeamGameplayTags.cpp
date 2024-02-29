// Copyright Project SunBeam. All Rights Reserved.


#include "Singleton/BeamGameplayTags.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

FBeamGameplayTags FBeamGameplayTags::GameplayTags;

void FBeamGameplayTags::InitializeGameplayTags()
{
	GameplayTags.LightSourceBase = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("LightSource"),
		FString("Light Source Base Tag, used to define interactions and visibility")
		);
	
	GameplayTags.LightSource_Sun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("LightSource.Sun"),
		FString("Gameplay Tag for Sun Light Source")
		);

	GameplayTags.LightSource_Moon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("LightSource.Moon"),
		FString("Gameplay Tag for Moon Light Source")
		);

	GameplayTags.SecondarySourceBase = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SecondarySource"),
		FString("Secondary Light Source Base Tag")
		);

	GameplayTags.SecondarySource_Sunflower = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SecondarySource.Sunflower"),
		FString("Gameplay Tag for Sunflower Secondary Light Source")
		);

	GameplayTags.SecondarySource_Lamp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("SecondarySource.Lamp"),
		FString("Gameplay Tag for Lamp Secondary Light Source")
		);

	GameplayTags.ObjectiveBase = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Objective"),
		FString("Gameplay Tag for Objective, used to define whether the owner is the level objective")
		);
}
