// Copyright Project SunBeam. All Rights Reserved.


#include "Beam/BeamActor.h"

#include "NiagaraComponent.h"

namespace SunBeamConsoleVariables
{
	static float DrawBeamTracesDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBeamTraceDuration(
		TEXT("Debug.BeamTraces.DrawBeamTraceDuration"),
		DrawBeamTracesDuration,
		TEXT("Should we do debug drawing for beam traces (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBeamHitDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBeamHits(
		TEXT("Debug.BeamTraces.DrawBeamHitDuration"),
		DrawBeamHitDuration,
		TEXT("Should we do debug drawing for Beam impacts (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBeamHitRadius = 3.0f;
	static FAutoConsoleVariableRef CVarDrawBeamHitRadius(
		TEXT("Debug.BeamTraces.DrawBeamHitRadius"),
		DrawBeamHitRadius,
		TEXT("When Beam hit debug drawing is enabled (see DrawBeamHitDuration), how big should the hit radius be? (in uu)"),
		ECVF_Default);
}

// Sets default values for this component's properties
ABeamActor::ABeamActor()
{
	PrimaryActorTick.bCanEverTick = true;

	BeamEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BeamComponent"));
	BeamEffectComponent->SetupAttachment(RootComponent);
	BeamEffectComponent->SetAutoActivate(false);
}

void ABeamActor::InitializeBeam(UNiagaraSystem* InBeamEffect, float InMaxBeamLength)
{
	UNiagaraSystem* BeamEffect = DefaultBeamEffect;
	if (InBeamEffect)
	{
		BeamEffect = InBeamEffect;
	}
	if (BeamEffect)
	{
		BeamEffectComponent->SetAsset(BeamEffect);
		BeamEffectComponent->Activate();
	}
	else
	{
		BeamEffectComponent->Deactivate();
	}

	if (InMaxBeamLength > 0)
	{
		MaxBeamLength = InMaxBeamLength;
	}
}

void ABeamActor::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultBeamEffect)
	{
		BeamEffectComponent->SetAsset(DefaultBeamEffect);
		BeamEffectComponent->Activate();
	}
}

void ABeamActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult BeamHitResult = RayTraceBeam();
	// TODO: If the beam hit something interactable, do something
}


FHitResult ABeamActor::RayTraceBeam() const
{
	// Trace from the actor's location towards forward
	FVector TraceStartLocation = GetActorLocation();
	FVector TraceEndLocation = TraceStartLocation + GetActorForwardVector() * MaxBeamLength;
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	
#if ENABLE_DRAW_DEBUG
	if (SunBeamConsoleVariables::DrawBeamTracesDuration > 0.0f)
	{
		static float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FColor::Red, false, SunBeamConsoleVariables::DrawBeamTracesDuration, 0, DebugThickness);
	}
#endif // ENABLE_DRAW_DEBUG
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		
#if ENABLE_DRAW_DEBUG
		if (SunBeamConsoleVariables::DrawBeamHitDuration > 0.0f)
		{
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, SunBeamConsoleVariables::DrawBeamHitRadius, FColor::Red, false, SunBeamConsoleVariables::DrawBeamHitDuration);
		}
#endif // ENABLE_DRAW_DEBUG
		
		TraceEndLocation = HitResult.ImpactPoint;
		return HitResult;
	}
	SetBeamEndLocation(TraceEndLocation);

	
	return FHitResult();
}

void ABeamActor::SetBeamEndLocation(const FVector& EndLocation) const
{
	BeamEffectComponent->SetVariableVec3(FName("Beam_end"), EndLocation);
	BeamEffectComponent->SetVariableVec3(FName("BeamScale"), FVector(0.5f, 0.5f, 10.0f));
}

