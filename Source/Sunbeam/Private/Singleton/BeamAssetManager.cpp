// Copyright Project SunBeam. All Rights Reserved.


#include "Singleton/BeamAssetManager.h"

UBeamAssetManager& UBeamAssetManager::Get()
{
	check(GEngine);
	UBeamAssetManager* BeamAssetManager = Cast<UBeamAssetManager>(GEngine->AssetManager);
	return *BeamAssetManager;
}

void UBeamAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}