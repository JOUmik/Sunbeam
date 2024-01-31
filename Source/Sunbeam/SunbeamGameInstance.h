// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "SunbeamGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUNBEAM_API USunbeamGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:
	void ChangeMap(int index);

	UPROPERTY(BlueprintReadOnly)
	int CurrentMapIndex = 0;

	bool HardwareControlEnabled = true;
};
