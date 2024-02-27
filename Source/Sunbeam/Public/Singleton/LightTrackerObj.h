// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LightTrackerObj.generated.h"

/**
 * 
 */
UCLASS()
class SUNBEAM_API ALightTrackerObj : public AActor
{
	GENERATED_BODY()
public:
	static ALightTrackerObj* GetInstance(UWorld* World);

	void AddBeamCount(const int32 Count);

	void AddSecondaryLightCount(const int32 Count);

	void SetMaterialScalarParameter(FName ParameterName, float Value) const;
	void SetMaterialVectorParameter(FName ParameterName, const FLinearColor& Value) const;

	virtual void BeginDestroy() override;
	
private:
	static ALightTrackerObj* Instance;

	int32 BeamCount = 0;
	int32 SecondaryLightCount = 0;
};

// Initialize the static member
ALightTrackerObj* ALightTrackerObj::Instance = nullptr;
