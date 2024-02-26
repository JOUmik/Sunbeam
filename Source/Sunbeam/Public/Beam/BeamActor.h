// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SceneComponent.h"
#include "BeamActor.generated.h"


class IBeamSpawner;
class IInteractable;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SUNBEAM_API ABeamActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ABeamActor();

	FORCEINLINE AActor* GetBeamOwner() const { return BeamOwner.Get(); }
	FORCEINLINE const FGameplayTag& GetBeamSourceTag() const { return BeamSourceTag; }
	
	void SetBeamOwner(AActor* InBeamOwner);
	void SetBeamSourceTag(const FGameplayTag& InBeamSourceTag);
	void SetBeamActiveStatus(bool bIsActive);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	bool RayTraceBeam(TArray<FHitResult>& OutHits) const;

	void SetBeamEndLocation(const FVector& EndLocation) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	float MaxBeamLength = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	float SweepRadius = 10.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Beam")
	TSet<AActor*> LastBeamHitInteractables;

private:
	bool CanInteractWithActor(AActor* OtherActor) const;

	UPROPERTY(BlueprintReadOnly, Category = "Beam Effects", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> BeamOwner;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Beam Effects", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> BeamEffectComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Beam Effects", meta = (AllowPrivateAccess = "true"))
	TArray<FHitResult> CurBeamHitResults;

	UPROPERTY(BlueprintReadOnly, Category = "Beam Effects", meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, FHitResult> CurBeamHitData;

	FGameplayTag BeamSourceTag;
};
