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
	
	SpawnBeamAttached(DefaultBeamEffect, DefaultMaxBeamLength);
}

// Called every frame
void ABeamPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABeamPawn::SpawnBeamAttached(UNiagaraSystem* BeamEffect, float MaxBeamLength)
{
	// Spawn Beam actor and attach it to this actor
	BeamActor = GetWorld()->SpawnActor<ABeamActor>(BeamActorClass, GetActorLocation(), GetActorRotation());
	BeamActor->InitializeBeam(BeamEffect, MaxBeamLength);
	BeamActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ABeamPawn::RotateBeamPawn(FVector2D RotateAxisVector)
{
	// Convert the current actor rotation to a quaternion
	FQuat CurrentQuat = GetActorQuat();

	// Create a quaternion for the yaw rotation around the global Z axis (up vector)
	FQuat YawQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(RotateAxisVector.X));

	// Create a quaternion for the pitch rotation around the global X axis (right vector)
	FQuat PitchQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(-RotateAxisVector.Y));

	// Apply the rotations additively by multiplying the quaternions with the current rotation quaternion
	// The order is important to ensure global axis rotations: first apply the yaw, then the pitch
	FQuat NewQuat = YawQuat * PitchQuat * CurrentQuat;

	// Normalize the resulting quaternion to ensure it represents a valid rotation
	NewQuat.Normalize();

	// Update the actor's rotation with the new quaternion
	SetActorRotation(NewQuat);
}

