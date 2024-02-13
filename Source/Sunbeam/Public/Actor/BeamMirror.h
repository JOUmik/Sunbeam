// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interface/BeamSpawner.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "BeamMirror.generated.h"

UCLASS()
class SUNBEAM_API ABeamMirror : public AActor, public IInteractable, public IBeamSpawner
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeamMirror();

	/* IInteractable interface */
	virtual void OnBeginInteract_Implementation(FHitResult BeamHitResult) override;
	virtual void OnEndInteract_Implementation() override;
	virtual void OnTickInteract_Implementation(FHitResult BeamHitResult, float DeltaTime) override;
	/* End IInteractable interface */

	/* IBeamSpawner interface */
	virtual void SpawnBeamActor_Implementation(TSubclassOf<class ABeamActor> BeamActorClass) override;
	/* End IBeamSpawner interface */

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	TSubclassOf<ABeamActor> DefaultBeamActorClass;

private:
	void UpdateBeamActorByHitData() const;
	
	TObjectPtr<ABeamActor> BeamActor;

	FHitResult CurBeamHitData; // TODO: Replace with custom struct that only contains the necessary data

	bool bIsBeingHit = false;
};
