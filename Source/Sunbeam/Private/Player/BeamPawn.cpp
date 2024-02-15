// Copyright Project SunBeam. All Rights Reserved.

#include "Player/BeamPawn.h"
#include "Beam/BeamActor.h"

// Sets default values
ABeamPawn::ABeamPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABeamPawn::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnBeamActor_Implementation(DefaultBeamActorClass);
}

// Called every frame
void ABeamPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABeamPawn::SpawnBeamActor_Implementation(TSubclassOf<ABeamActor> BeamActorClass)
{
	// Spawn Beam actor and attach it to this actor
	check(BeamActorClass)
	BeamActor = GetWorld()->SpawnActor<ABeamActor>(BeamActorClass, GetActorLocation(), GetActorRotation());
	BeamActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

ABeamActor* ABeamPawn::GetSpawnedBeamActor_Implementation()
{
	return BeamActor;
}

void ABeamPawn::RotateBeamPawn(FVector2D RotateAxisVector)
{
	// Convert the current actor rotation to a quaternion
	const FQuat CurrentQuat = GetActorQuat();

	// Create a quaternion for the yaw rotation around the global Z axis (up vector)
	const FQuat YawQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(RotateAxisVector.X));

	// Create a quaternion for the pitch rotation around the global X axis (right vector)
	const FQuat PitchQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(-RotateAxisVector.Y));

	// Apply the rotations additively by multiplying the quaternions with the current rotation quaternion
	FQuat NewQuat = YawQuat * PitchQuat * CurrentQuat;

	// Normalize the resulting quaternion to ensure it represents a valid rotation
	NewQuat.Normalize();

	// Update the actor's rotation with the new quaternion
	SetActorRotation(NewQuat);
}
