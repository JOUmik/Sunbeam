// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Beam/IBeamSpawner.h"
#include "GameFramework/Pawn.h"
#include "BeamPawn.generated.h"

class ABeamActor;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SUNBEAM_API ABeamPawn : public APawn, public IBeamSpawner
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABeamPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* IBeamSpawner interface */
	virtual void SpawnBeamAttached(UNiagaraSystem* BeamEffect, float MaxBeamLength) override;
	/* End IBeamSpawner interface */

	// Rotate the beam
	void RotateBeamPawn(FVector2D RotateAxisVector);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	TObjectPtr<UNiagaraSystem> DefaultBeamEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	TSubclassOf<ABeamActor> BeamActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	float DefaultMaxBeamLength = 100.0f;

private:
	TObjectPtr<ABeamActor> BeamActor;
};
