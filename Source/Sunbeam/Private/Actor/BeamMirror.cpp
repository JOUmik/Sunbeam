// Copyright Project SunBeam. All Rights Reserved.


#include "Actor/BeamMirror.h"

#include "Beam/BeamActor.h"

// Sets default values
ABeamMirror::ABeamMirror()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABeamMirror::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABeamMirror::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABeamMirror::OnBeginInteract_Implementation(FHitResult BeamHitResult, FGameplayTag BeamLightSourceTag)
{
	/* TODO: Added a check so that new beam will not hit this mirror and spawn another new beam infinitely;
	 * But current solution doesn't support a mirror to be hit by multiple beams at the same time.
	 * Should add a way to have spawned beam by this mirror to ignore this mirror's collision.
	 */
	
	if (bIsBeingHit)
	{
		return;
	}
	// Spawn the beam at hit location, and set the beam's end location to the mirror's reflection location
	CurBeamHitData = BeamHitResult;
	SpawnBeamActor_Implementation(DefaultBeamActorClass);
	UpdateBeamActorByHitData();
	bIsBeingHit = true;
}

void ABeamMirror::OnEndInteract_Implementation()
{
	BeamActor->Destroy();
	bIsBeingHit = false;
}

void ABeamMirror::OnTickInteract_Implementation(FHitResult BeamHitResult, FGameplayTag BeamLightSourceTag, float DeltaTime)
{
	CurBeamHitData = BeamHitResult;
	UpdateBeamActorByHitData();
}

void ABeamMirror::GetInteractableTags_Implementation(FGameplayTagContainer& OutTagContainer)
{
	OutTagContainer = InteractableTags;
}

void ABeamMirror::SpawnBeamActor_Implementation(TSubclassOf<ABeamActor> BeamActorClass)
{
	check(BeamActorClass)
	
	BeamActor = GetWorld()->SpawnActor<ABeamActor>(BeamActorClass, GetActorLocation(), GetActorRotation());
}

ABeamActor* ABeamMirror::GetSpawnedBeamActor_Implementation()
{
	return BeamActor;
}

void ABeamMirror::UpdateBeamActorByHitData() const
{
	if (!IsValid(BeamActor))
	{
		return;
	}
	// Set the beam start at beam hit point
	BeamActor->SetActorLocation(CurBeamHitData.ImpactPoint);

	// Calculate the reflected beam direction
	const FVector IncomingBeamDirection = CurBeamHitData.ImpactPoint - CurBeamHitData.TraceStart;
	const FVector ImpactNormal = CurBeamHitData.ImpactNormal;
	const FVector ReflectedBeamDirection = IncomingBeamDirection.MirrorByVector(ImpactNormal);

	// Set the beam rotation to match the reflected beam direction
	BeamActor->SetActorRotation(ReflectedBeamDirection.Rotation());
}
