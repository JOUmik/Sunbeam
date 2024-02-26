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

	// Spawn all beam actors
	for (const auto& It : BeamActorClasses)
	{
		CurBeamTag = It.Key;
		ABeamActor* SpawnedBeamActor = SpawnBeamActor_Implementation(It.Value, CurBeamTag);
		OwningBeamActors.Add(It.Key, SpawnedBeamActor);
		CurBeamActor = SpawnedBeamActor;
	}

	// Set the default beam actor
	// After spawning all beam actors, cur beam tag is the last tag in the map
	// So after switching, cur beam tag will be the first tag in the map
	SwitchToNextBeamState();
}

// Called every frame
void ABeamPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ABeamActor* ABeamPawn::SpawnBeamActor_Implementation(TSubclassOf<ABeamActor> BeamActorClass, FGameplayTag& BeamSourceTag)
{
	// Spawn Beam actor and attach it to this actor
	check(BeamActorClass)

	ABeamActor* BeamActor = GetWorld()->SpawnActor<ABeamActor>(BeamActorClass, GetActorLocation(), GetActorRotation());
	BeamActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
	BeamActor->SetBeamOwner(this);
	BeamActor->SetBeamSourceTag(BeamSourceTag);
	return BeamActor;
}

ABeamActor* ABeamPawn::GetOwningBeamActor_Implementation()
{
	return CurBeamActor;
}

void ABeamPawn::RotateBeamPawn_MouseInput(FVector2D RotateAxisVector)
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

void ABeamPawn::RotateBeamPawn_ControllerInput(FVector2D RotateAxisVector)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Rotate: %f, %f"), RotateAxisVector.X, RotateAxisVector.Y));

	//Calculate the Angle
	FVector2D Base(0, -1);
	double Yaw = FMath::Atan2(RotateAxisVector.Y, RotateAxisVector.X) - FMath::Atan2(Base.Y, Base.X);
	Yaw = FMath::RadiansToDegrees(Yaw);
	if (Yaw <= 0) Yaw = FMath::Abs(Yaw);
	else Yaw = 360.0f - Yaw;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Yaw: %f"), Yaw));

	//Calculate thr Height Of Sun. (SunHeight == 0: Highest, SunHeight == 1: Lowest)
	double SunHeight = FMath::Sqrt(FMath::Square(RotateAxisVector.X) + FMath::Square(RotateAxisVector.Y));
	SunHeight = FMath::Min(SunHeight, 1.0f);
	double YRotation;
	//if (Input.Y >= 0) 
	YRotation = 270 + SunHeight * 90;
	//else YRotation = 270 + SunHeight * 90;
	//FRotator CurRotator = ControledLight->GetActorRotation();
	FRotator rotator(YRotation, Yaw, 0);
		
	SetActorRotation(rotator);
}

void ABeamPawn::SwitchToNextBeamState()
{
	auto It = OwningBeamActors.CreateConstIterator();
	check(It);

	while (It)
	{
		if (It.Key().MatchesTag(CurBeamTag))
		{
			// Disable the current beam actor
			if (IsValid(CurBeamActor))
			{
				CurBeamActor->SetBeamActiveStatus(false);
			}
			
			++It;
			if (!It)
			{
				// Set the next beam actor in map active
				CurBeamTag = OwningBeamActors.CreateConstIterator().Key();
				CurBeamActor = OwningBeamActors.CreateConstIterator().Value();
			}
			else
			{
				// Reaches the end of the map, set the first beam actor
				CurBeamTag = It.Key();
				CurBeamActor = It.Value();
			}

			CurBeamActor->SetBeamActiveStatus(true);
			break;
		}
		else
		{
			++It;
		}
	}
}
