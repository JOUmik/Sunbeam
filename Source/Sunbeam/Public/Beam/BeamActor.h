// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BeamActor.generated.h"


class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class SUNBEAM_API ABeamActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ABeamActor();

	void InitializeBeam(UNiagaraSystem* BeamEffect = nullptr, float MaxBeamLength = 0.0f);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
	FHitResult RayTraceBeam() const;

	void SetBeamEndLocation(const FVector& EndLocation) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam Effects")
	TObjectPtr<UNiagaraSystem> DefaultBeamEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Beam Effects")
	float MaxBeamLength = 100.0f;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Beam Effects", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> BeamEffectComponent;
};
