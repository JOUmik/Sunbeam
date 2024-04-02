// Copyright Project SunBeam. All Rights Reserved.


#include "Actor/BeamFlowerBase.h"

#include "ActorComponent/BeamEnergyStorageComponent.h"
#include "Beam/BeamActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/BeamPawn.h"

// Sets default values
ABeamFlowerBase::ABeamFlowerBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BeamEnergyStorageComponent = CreateDefaultSubobject<UBeamEnergyStorageComponent>(TEXT("BeamEnergyStorageComponent"));
	BeamEnergyStorageComponent->MaxEnergy = EnergyValue;
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(150.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(SphereComponent);
}

void ABeamFlowerBase::GetInteractableResponseTags_Implementation(FGameplayTagContainer& OutTagContainer)
{
	OutTagContainer = InteractableResponseTags;
}

void ABeamFlowerBase::OnBeginInteract_Implementation(FHitResult LightHitResult, AActor* LightSource)
{
	if (!LightSource->GetClass()->ImplementsInterface(ULightSource::StaticClass()))
	{
		return;
	}
	
	CurInteractingBeamSourceTag = ILightSource::Execute_GetLightSourceTag(LightSource);
	if (!CanInteractWithBeam())
	{
		return;
	}
	
	bIsChangingStatus = true;
	bHasBloomed ? PlayBloomAnimReverse() : PlayBloomAnimForward();
}

void ABeamFlowerBase::OnEndInteract_Implementation()
{
	if (!bIsChangingStatus)
	{
		return;
	}
	
	bHasBloomed ? PlayBloomAnimForward() : PlayBloomAnimReverse();
}

// Called when the game starts or when spawned
void ABeamFlowerBase::BeginPlay()
{
	Super::BeginPlay();
}

// When the bloom animation reaches the end, the flower is in bloom. When the bloom animation was played in reverse and reaches the start, the flower is not in bloom.
void ABeamFlowerBase::SetBloomStatus(const bool bBloomed)
{
	if (bBloomed == bHasBloomed)
	{
		return;
	}
	
	bHasBloomed = bBloomed;
	bIsChangingStatus = false;

	const ABeamPawn* BeamPawn = Cast<ABeamPawn>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (!IsValid(BeamPawn))
	{
		return;
	}

	const bool bSuccess = bHasBloomed ? ConsumeEnergyFromPlayer() : AddEnergyToPlayer();

	// Energy was checked before transfer, assert here to help capture boundary cases
	check(bSuccess);

	OnFlowerChangeBloomStatusDelegate.Broadcast(bHasBloomed);
}

bool ABeamFlowerBase::ConsumeEnergyFromPlayer() const
{
	const ABeamPawn* PlayerPawn = Cast<ABeamPawn>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	UBeamEnergyStorageComponent* PlayerEnergyStorageComponent = PlayerPawn->GetBeamEnergyStorageComponent();
	if (!IsValid(PlayerEnergyStorageComponent) || !IsValid(BeamEnergyStorageComponent))
	{
		return false;
	}

	bool bSuccess = PlayerEnergyStorageComponent->ConsumeEnergy(BeamEnergyStorageComponent->GetMaxEnergy(), CurInteractingBeamSourceTag);

	// For flowers, energy is not stored per tag, so we need to add energy for all interactable tags the flower has
	for (auto& InteractableTag : InteractableResponseTags)
	{
		bSuccess &= BeamEnergyStorageComponent->AddEnergy(BeamEnergyStorageComponent->GetMaxEnergy(), InteractableTag);
	}
	
	return bSuccess;
}

bool ABeamFlowerBase::AddEnergyToPlayer() const
{
	const ABeamPawn* PlayerPawn = Cast<ABeamPawn>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	UBeamEnergyStorageComponent* PlayerEnergyStorageComponent = PlayerPawn->GetBeamEnergyStorageComponent();
	if (!IsValid(PlayerEnergyStorageComponent) || !IsValid(BeamEnergyStorageComponent))
	{
		return false;
	}

	bool bSuccess = PlayerEnergyStorageComponent->AddEnergy(BeamEnergyStorageComponent->GetMaxEnergy(), CurInteractingBeamSourceTag);

	// For flowers, energy is not stored per tag, so we need to consume energy for all interactable tags the flower has
	for (auto& InteractableTag : InteractableResponseTags)
	{
		bSuccess &= BeamEnergyStorageComponent->ConsumeEnergy(BeamEnergyStorageComponent->GetMaxEnergy(), InteractableTag);
	}
	
	return bSuccess;
}

// Check if the flower can interact with the beam from the energy perspective
bool ABeamFlowerBase::CanInteractWithBeam() const
{
	const ABeamPawn* PlayerPawn = Cast<ABeamPawn>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	const UBeamEnergyStorageComponent* PlayerEnergyStorageComponent = PlayerPawn->GetBeamEnergyStorageComponent();
	if (!IsValid(PlayerEnergyStorageComponent) || !IsValid(BeamEnergyStorageComponent))
	{
		return false;
	}

	if (bHasBloomed)
	{
		// About to add energy to the player and consume energy from the flower
		return BeamEnergyStorageComponent->GetCurEnergy(CurInteractingBeamSourceTag) > 0 && PlayerEnergyStorageComponent->GetCurEnergy(CurInteractingBeamSourceTag) + BeamEnergyStorageComponent->GetCurEnergy(CurInteractingBeamSourceTag) <= PlayerEnergyStorageComponent->GetMaxEnergy();
	}
	else
	{
		// About to consume energy from the player and add energy to the flower
		return BeamEnergyStorageComponent->GetCurEnergy(CurInteractingBeamSourceTag) < BeamEnergyStorageComponent->GetMaxEnergy() && PlayerEnergyStorageComponent->GetCurEnergy(CurInteractingBeamSourceTag) >= BeamEnergyStorageComponent->GetMaxEnergy();
	}
}