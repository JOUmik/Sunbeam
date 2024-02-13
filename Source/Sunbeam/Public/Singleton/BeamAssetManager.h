// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BeamAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SUNBEAM_API UBeamAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UBeamAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
