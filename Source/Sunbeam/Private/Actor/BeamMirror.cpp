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

void ABeamMirror::OnBeginInteract_Implementation(const FHitResult LightHitResult, AActor* LightSource)
{
	if (bIsBeingHit)
	{
		return;
	}
	const ABeamActor* BeamActor = Cast<ABeamActor>(LightSource);
	if (!BeamActor)
	{
		return;
	}
	
	// Spawn the beam at hit location, and set the beam's end location to the mirror's reflection location
	CurBeamHitData = LightHitResult;

	const TSubclassOf<ABeamActor> BeamActorClass = BeamActor->GetClass();
	FGameplayTag BeamSourceTag = BeamActor->GetBeamSourceTag();
	AActor* BeamOwner = BeamActor->GetBeamOwner();
	SpawnedBeamActor = SpawnBeamActor_Implementation(BeamActorClass, BeamSourceTag);
	SpawnedBeamActor->SetBeamOwner(BeamOwner);
	SpawnedBeamActor->SetBeamActiveStatus(true);
	UpdateBeamActorByHitData();
	bIsBeingHit = true;
}

void ABeamMirror::OnEndInteract_Implementation()
{
	SpawnedBeamActor->SetBeamActiveStatus(false);
	SpawnedBeamActor->Destroy();
	bIsBeingHit = false;
}

void ABeamMirror::OnTickInteract_Implementation(const FHitResult LightHitResult, AActor* LightSource, float DeltaTime)
{
	CurBeamHitData = LightHitResult;
	UpdateBeamActorByHitData();
}

void ABeamMirror::GetInteractableTags_Implementation(FGameplayTagContainer& OutTagContainer)
{
	OutTagContainer = InteractableTags;
}

ABeamActor* ABeamMirror::SpawnBeamActor_Implementation(TSubclassOf<ABeamActor> BeamActorClass, FGameplayTag& BeamSourceTag)
{
	check(BeamActorClass)
	
	return GetWorld()->SpawnActor<ABeamActor>(BeamActorClass, GetActorLocation(), GetActorRotation());
}

ABeamActor* ABeamMirror::GetOwningBeamActor_Implementation()
{
	return SpawnedBeamActor;
}

void ABeamMirror::UpdateBeamActorByHitData() const
{
	if (!IsValid(SpawnedBeamActor))
	{
		return;
	}
	// Set the beam start at beam hit point
	SpawnedBeamActor->SetActorLocation(CurBeamHitData.ImpactPoint);

	// Calculate the reflected beam direction
	const FVector IncomingBeamDirection = CurBeamHitData.ImpactPoint - CurBeamHitData.TraceStart;
	const FVector ImpactNormal = CurBeamHitData.ImpactNormal;
	const FVector ReflectedBeamDirection = IncomingBeamDirection.MirrorByVector(ImpactNormal);

	// Set the beam rotation to match the reflected beam direction
	SpawnedBeamActor->SetActorRotation(ReflectedBeamDirection.Rotation());
}
