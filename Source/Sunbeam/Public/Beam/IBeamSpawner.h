// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBeamSpawner.generated.h"

class UNiagaraSystem;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBeamSpawner : public UInterface
{
	GENERATED_BODY()
};

/**
 *  Interface for spawning and updating a beam effect
 *  For now this is used by the BeamPawn to spawn and update the beam as per user input
 *  And by mirrors that spawn and update reflected beam as per the beam's movement
 */
class SUNBEAM_API IBeamSpawner
{
	GENERATED_BODY()

public:
	virtual void SpawnBeamAttached(UNiagaraSystem* BeamEffect, float MaxBeamLength) = 0;
};
