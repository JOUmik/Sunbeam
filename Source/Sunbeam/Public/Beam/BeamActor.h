// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SceneComponent.h"
#include "BeamActor.generated.h"


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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	bool RayTraceBeam(FHitResult& CurHitResult) const;

	void SetBeamEndLocation(const FVector& EndLocation) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	float MaxBeamLength = 100.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Beam")
	AActor* LastBeamHitActor;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Beam")
	AActor* CurBeamHitActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	FGameplayTag BeamSourceTag;

private:
	bool CanInteractWithActor(AActor* OtherActor) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Beam Effects", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> BeamEffectComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Beam Effects", meta = (AllowPrivateAccess = "true"))
	FHitResult CurBeamHitResult;
};
