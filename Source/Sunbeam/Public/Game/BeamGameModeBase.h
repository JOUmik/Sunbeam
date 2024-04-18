// Project SunBeam, all rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "BeamGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveCompletedCountChanged, int32, NewCompletedCount, int32, NewObjectiveCount);

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

	int32 GetObjectiveCount() const;
	int32 GetObjectiveCompletedCount() const;
	bool IsObjectiveCompleted() const;

	UFUNCTION()
	void OnObjectiveStateChanged(bool bNewState);

	UPROPERTY(EditDefaultsOnly, Category = "Directional Light")
	float SunlightIntensity = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Directional Light")
	float MoonLightIntensity = 1.0f;

	FOnObjectiveCompletedCountChanged OnObjectiveCompletedCountChangeDelegate;

private:
	int32 BeamCount = 0;
	int32 SecondaryLightCount = 0;

	int32 ObjectiveCount;
	int32 ObjectiveCompletedCount;

	bool bIsObjectiveCompleted = false;

	TMap<FGameplayTag, TArray<AActor*>> InteractableMap;
};
