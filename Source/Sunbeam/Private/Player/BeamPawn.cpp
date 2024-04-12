// Copyright Project SunBeam. All Rights Reserved.

#include "Player/BeamPawn.h"

#include "ActorComponent/BeamEnergyStorageComponent.h"
#include "Beam/BeamActor.h"
#include "Game/BeamGameModeBase.h"
#include "Player/BeamPlayerController.h"
#include "UI/BeamHUD.h"

// Sets default values
ABeamPawn::ABeamPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BeamEnergyStorageComponent = CreateDefaultSubobject<UBeamEnergyStorageComponent>(TEXT("BeamEnergyStorageComponent"));
	BeamEnergyStorageComponent->OnBeamEnergyChangedDelegate.AddDynamic(this, &ABeamPawn::OnEnergyChanged);
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

	const ABeamPlayerController* BeamPlayerController = Cast<ABeamPlayerController>(GetController());
	if (ABeamHUD* BeamHUD = Cast<ABeamHUD>(BeamPlayerController->GetHUD()))
	{
		BeamHUD->InitOverlay(BeamEnergyStorageComponent, GetWorld()->GetAuthGameMode<ABeamGameModeBase>());
	}
}

void ABeamPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABeamPawn::OnEnergyChanged(int32 NewCurEnergy, FGameplayTag EnergyTypeTag)
{
	// TODO: Change the light radius based on the current energy
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
	// Get the current rotation of the actor
	const FQuat CurrentRotation = GetActorQuat();

	// Calculate the new rotation based on the mouse input
	FQuat DeltaRotation(FVector::RightVector, FMath::DegreesToRadians(-RotateAxisVector.Y));
	DeltaRotation *= FQuat(FVector::UpVector, FMath::DegreesToRadians(RotateAxisVector.X));

	// Set the new rotation to the actor
	SetActorRotation(CurrentRotation * DeltaRotation);
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

			const ABeamGameModeBase* BeamGameMode = GetWorld()->GetAuthGameMode<ABeamGameModeBase>();
			check(BeamGameMode);
			BeamGameMode->ShowInteractableByType(CurBeamTag);
			
			CurBeamActor->SetBeamActiveStatus(true);
			break;
		}
		else
		{
			++It;
		}
	}
}
