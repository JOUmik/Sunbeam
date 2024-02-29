// Project SunBeam, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "BeamGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SUNBEAM_API ABeamGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	void AddBeamCount(const int32 Count);
	void AddSecondaryLightCount(const int32 Count);

	void SetMaterialScalarParameter(FName ParameterName, float Value) const;
	void SetMaterialVectorParameter(FName ParameterName, const FLinearColor& Value) const;
	
	void BufferInteractableByType();
	void ShowInteractableByType(const FGameplayTag& InteractableType) const;

	UFUNCTION()
	void OnObjectiveStateChanged(bool bNewState);

private:
	int32 BeamCount = 0;
	int32 SecondaryLightCount = 0;

	int32 ObjectiveCount;
	int32 ObjectiveCompletedCount;

	TMap<FGameplayTag, TArray<AActor*>> InteractableMap;
};
