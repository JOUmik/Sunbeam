// Copyright Project SunBeam. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Interface/BeamSpawner.h"
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
	virtual ABeamActor* SpawnBeamActor_Implementation(TSubclassOf<ABeamActor> BeamActorClass, FGameplayTag& BeamSourceTag) override;
	virtual ABeamActor* GetOwningBeamActor_Implementation() override;
	/* End IBeamSpawner interface */

	// Rotate the beam
	void RotateBeamPawn_MouseInput(FVector2D RotateAxisVector);
	void RotateBeamPawn_ControllerInput(FVector2D RotateAxisVector);

	// Move to next beam state
	void SwitchToNextBeamState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Beam")
	TMap<FGameplayTag, TSubclassOf<ABeamActor>> BeamActorClasses;
	
private:
	TMap<FGameplayTag, TObjectPtr<ABeamActor>> OwningBeamActors;

	TObjectPtr<ABeamActor> CurBeamActor;
	
	FGameplayTag CurBeamTag;
};
