// Copyright Project SunBeam. All Rights Reserved.


#include "Actor/BeamGlowingFlower.h"

#include "Sunbeam.h"
#include "Components/SphereComponent.h"
#include "Game/BeamGameModeBase.h"

namespace SunBeamConsoleVariables
{
	static float DrawSecondaryLightRadiusDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawSecondaryLightRadiusDuration(
		TEXT("Debug.LightSource.DrawSecondaryLightRadiusDuration"),
		DrawSecondaryLightRadiusDuration,
		TEXT("Should we do debug drawing for secondary light source radius (if above zero, sets how long (in seconds))"),
		ECVF_Default);
}


ABeamGlowingFlower::ABeamGlowingFlower()
{
	OnFlowerChangeBloomStatusDelegate.AddDynamic(this, &ABeamGlowingFlower::OnBloomStatusChanged);

	GlowingRadiusComponent = CreateDefaultSubobject<USphereComponent>(TEXT("GlowingRadius"));
	GlowingRadiusComponent->SetupAttachment(RootComponent);
	GlowingRadiusComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GlowingRadiusComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	GlowingRadiusComponent->SetCollisionResponseToChannel(ECC_Light, ECR_Ignore);
	GlowingRadiusComponent->OnComponentBeginOverlap.AddDynamic(this, &ABeamGlowingFlower::OnGlowingRadiusBeginOverlap);
	GlowingRadiusComponent->OnComponentEndOverlap.AddDynamic(this, &ABeamGlowingFlower::OnGlowingRadiusEndOverlap);
	GlowingRadiusComponent->SetSphereRadius(100.0f);

	PrimaryActorTick.bCanEverTick = true;
}

void ABeamGlowingFlower::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void ABeamGlowingFlower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (AActor* OverlappingActor : CurOverlappingInteractables)
	{
		Execute_OnTickInteract(OverlappingActor, FHitResult(), this, DeltaSeconds);
	}
}

FGameplayTag ABeamGlowingFlower::GetLightSourceTag_Implementation() const
{
	return SecondaryLightSourceTag;
}

bool ABeamGlowingFlower::CanInteractWithActor_Implementation(AActor* OtherActor) const
{
	if (!IsValid(OtherActor) or !OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return false;
	}

	FGameplayTagContainer OtherInteractableTags;
	IInteractable::Execute_GetInteractableResponseTags(OtherActor, OtherInteractableTags);
	return OtherInteractableTags.HasTag(SecondaryLightSourceTag);
}

void ABeamGlowingFlower::OnGlowingRadiusBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CanInteractWithActor_Implementation(OtherActor))
	{
		CurOverlappingInteractables.Add(OtherActor);
		Execute_OnBeginInteract(OtherActor, SweepResult, this);
	}
}

void ABeamGlowingFlower::OnGlowingRadiusEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
if (CanInteractWithActor_Implementation(OtherActor))
	{
		CurOverlappingInteractables.Remove(OtherActor);
		Execute_OnEndInteract(OtherActor);
	}
}

void ABeamGlowingFlower::OnBloomStatusChanged(const bool bBloomed)
{
	// Set enable tick
	SetActorTickEnabled(bBloomed);

	GlowingRadiusComponent->SetCollisionEnabled(bBloomed ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);

	for (AActor* OverlappingActor : CurOverlappingInteractables)
	{
		if (!bBloomed)
		{
			Execute_OnEndInteract(OverlappingActor);
		}
	}
	CurOverlappingInteractables.Empty();

	ABeamGameModeBase* BeamGameMode = GetWorld()->GetAuthGameMode<ABeamGameModeBase>();
	check(BeamGameMode);
	BeamGameMode->AddSecondaryLightCount(bBloomed ? 1 : -1);
}

