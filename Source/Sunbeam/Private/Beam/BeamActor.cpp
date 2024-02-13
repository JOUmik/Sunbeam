// Copyright Project SunBeam. All Rights Reserved.


#include "Beam/BeamActor.h"

#include "NiagaraComponent.h"
#include "Interface/Interactable.h"

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

	CurBeamHitResult = FHitResult();
}

void ABeamActor::BeginPlay()
{
	Super::BeginPlay();

	BeamEffectComponent->Activate();
}

void ABeamActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	RayTraceBeam(CurBeamHitResult);

	// No need to check if the beam hits anything, cause if it doesn't, we just set the cur beam hit actor to nullptr
	// will still trigger the end interact event if the last beam hit actor was valid
	CurBeamHitActor = CurBeamHitResult.GetActor();
	
	if (CurBeamHitActor != LastBeamHitActor)
	{
		if (CanInteractWithActor(LastBeamHitActor))
		{
			IInteractable::Execute_OnEndInteract(LastBeamHitActor);
		}
		if (CanInteractWithActor(CurBeamHitActor))
		{
			IInteractable::Execute_OnBeginInteract(CurBeamHitActor, CurBeamHitResult, BeamSourceTag);
		}
		LastBeamHitActor = CurBeamHitActor;
	}
	else
	{
		FString LastActorName = LastBeamHitActor ? LastBeamHitActor->GetName() : TEXT("None");
		FString HitActorName = CurBeamHitActor ? CurBeamHitActor->GetName() : TEXT("None");
		if (CanInteractWithActor(CurBeamHitActor))
		{
			IInteractable::Execute_OnTickInteract(CurBeamHitActor, CurBeamHitResult, BeamSourceTag, DeltaSeconds);
		}
	}
}

// TODO: Raytracing beam support sweep and multi-hit
bool ABeamActor::RayTraceBeam(FHitResult& CurHitResult) const
{
	// Trace from the actor's location towards forward
	const FVector TraceStartLocation = GetActorLocation();
	FVector TraceEndLocation = TraceStartLocation + GetActorForwardVector() * MaxBeamLength;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	
#if ENABLE_DRAW_DEBUG
	if (SunBeamConsoleVariables::DrawBeamTracesDuration > 0.0f)
	{
		static float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FColor::Red, false, SunBeamConsoleVariables::DrawBeamTracesDuration, 0, DebugThickness);
	}
#endif // ENABLE_DRAW_DEBUG
	
	if (GetWorld()->LineTraceSingleByChannel(CurHitResult, TraceStartLocation, TraceEndLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
#if ENABLE_DRAW_DEBUG
		if (SunBeamConsoleVariables::DrawBeamHitDuration > 0.0f)
		{
			DrawDebugPoint(GetWorld(), CurHitResult.ImpactPoint, SunBeamConsoleVariables::DrawBeamHitRadius, FColor::Red, false, SunBeamConsoleVariables::DrawBeamHitDuration);
		}
#endif // ENABLE_DRAW_DEBUG
		
		TraceEndLocation = CurHitResult.ImpactPoint;
		SetBeamEndLocation(TraceEndLocation);
		return true;
	}
	else
	{
		// If we didn't hit anything, set the beam end to the max length
		SetBeamEndLocation(TraceEndLocation);
		return false;
	}
}

void ABeamActor::SetBeamEndLocation(const FVector& EndLocation) const
{
	BeamEffectComponent->SetVariableVec3(FName("Beam_end"), EndLocation);
	BeamEffectComponent->SetVariableVec3(FName("BeamScale"), FVector(0.5f, 0.5f, 10.0f));
}

bool ABeamActor::CanInteractWithActor(AActor* OtherActor) const
{
	if (!IsValid(OtherActor))
	{
		return false;
	}
	
	if(!OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return false;
	}

	// Check if the beam source tag is in the interactable tags
	const IInteractable* Interactable = Cast<IInteractable>(OtherActor);
	const FGameplayTagContainer& InteractableTags = Interactable->GetInteractableTags();
	return InteractableTags.HasTag(BeamSourceTag);
}
