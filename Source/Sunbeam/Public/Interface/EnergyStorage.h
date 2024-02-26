// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnergyStorage.generated.h"

class UBeamEnergyStorageComponent;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnergyStorage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SUNBEAM_API IEnergyStorage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UBeamEnergyStorageComponent* GetBeamEnergyStorageComponent() const = 0;
	
};
