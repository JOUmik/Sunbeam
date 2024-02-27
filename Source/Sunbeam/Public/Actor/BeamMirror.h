// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interface/BeamSpawner.h"
#include "Interface/Interactable.h"
#include "Interface/LightSource.h"
#include "BeamMirror.generated.h"

UCLASS()
class SUNBEAM_API ABeamMirror : public AActor, public IInteractable, public IBeamSpawner
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABeamMirror();

	/* IInteractable interface */
	virtual void OnBeginInteract_Implementation(FHitResult LightHitResult, AActor* LightSource) override;
	virtual void OnEndInteract_Implementation() override;
	virtual void OnTickInteract_Implementation(FHitResult LightHitResult, AActor* LightSource, float DeltaTime) override;
	virtual void GetInteractableTags_Implementation(FGameplayTagContainer& OutTagContainer) override;
	/* End IInteractable interface */

	/* IBeamSpawner interface */
	virtual ABeamActor* SpawnBeamActor_Implementation(TSubclassOf<ABeamActor> BeamActorClass, FGameplayTag& BeamSourceTag) override;
	virtual ABeamActor* GetOwningBeamActor_Implementation() override;
	/* End IBeamSpawner interface */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam Interaction")
	FGameplayTagContainer InteractableTags;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void UpdateBeamActorByHitData() const;
	

private:
	TObjectPtr<ABeamActor> SpawnedBeamActor;

	FHitResult CurBeamHitData; // TODO: Replace with custom struct that only contains the necessary data

	bool bIsBeingHit = false;
};
