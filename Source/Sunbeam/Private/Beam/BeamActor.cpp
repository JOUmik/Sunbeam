// Copyright Project SunBeam. All Rights Reserved.


#include "Beam/BeamActor.h"

#include "NiagaraComponent.h"
#include "Sunbeam.h"
#include "ActorComponent/BeamEnergyStorageComponent.h"
#include "Interface/BeamSpawner.h"
#include "Interface/Interactable.h"
#include "Player/BeamPawn.h"
#include "Singleton/LightTrackerObj.h"

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

void ABeamActor::BeginPlay()
{
	Super::BeginPlay();

	BeamEffectComponent->Activate();
}

void ABeamActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	RayTraceBeam(CurBeamHitResults);

	TSet<AActor*> CurBeamHitInteractables;
	for (const FHitResult& HitResult : CurBeamHitResults)
	{
		
#if ENABLE_DRAW_DEBUG
		if (SunBeamConsoleVariables::DrawBeamHitDuration > 0.0f)
		{
			DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, SunBeamConsoleVariables::DrawBeamHitRadius, FColor::Red, false, SunBeamConsoleVariables::DrawBeamHitDuration);
			
			// Draw the normal of the hit
			DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.ImpactPoint + HitResult.ImpactNormal * 100.0f, FColor::Yellow, false, SunBeamConsoleVariables::DrawBeamHitDuration);
		}
#endif // ENABLE_DRAW_DEBUG
		
		if (CanInteractWithActor_Implementation(HitResult.GetActor()))
		{
			CurBeamHitInteractables.Add(HitResult.GetActor());
			CurBeamHitData.Add(HitResult.GetActor(), HitResult);
		}
	}
	
	TSet<AActor*> OldActors = CurBeamHitInteractables.Intersect(LastBeamHitInteractables);
	TSet<AActor*> NewActors = CurBeamHitInteractables.Difference(LastBeamHitInteractables);
	TSet<AActor*> RemovedActors = LastBeamHitInteractables.Difference(CurBeamHitInteractables);

	for (AActor* RemovedActor : RemovedActors)
	{
		IInteractable::Execute_OnEndInteract(RemovedActor);
	}

	for (AActor* NewActor : NewActors)
	{
		FHitResult CurBeamHitResult = CurBeamHitData[NewActor];
		IInteractable::Execute_OnBeginInteract(NewActor, CurBeamHitResult, this);
	}

	for (AActor* OldActor : OldActors)
	{
		FHitResult CurBeamHitResult = CurBeamHitData[OldActor];
		IInteractable::Execute_OnTickInteract(OldActor, CurBeamHitResult, this, DeltaSeconds);
	}

	LastBeamHitInteractables = CurBeamHitInteractables;
}

bool ABeamActor::RayTraceBeam(TArray<FHitResult>& OutHits) const
{
	// Trace from the actor's location towards forward
	const FVector TraceStartLocation = GetActorLocation();
	FVector TraceEndLocation = TraceStartLocation + GetActorForwardVector() * MaxBeamLength;
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetBeamOwner());
	CollisionParams.bTraceComplex = true;
	
	bool bHit;
	if (SweepRadius > 0.0f)
	{
		bHit = GetWorld()->SweepMultiByChannel(OutHits, TraceStartLocation, TraceEndLocation, FQuat::Identity, ECC_Light, FCollisionShape::MakeSphere(SweepRadius), CollisionParams);
	}
	else
	{
		bHit = GetWorld()->LineTraceMultiByChannel(OutHits, TraceStartLocation, TraceEndLocation, ECC_Light, CollisionParams);
	}

	if (OutHits.Num() > 0)
	{
		TraceEndLocation = OutHits.Last().ImpactPoint;
	}

#if ENABLE_DRAW_DEBUG
	if (SunBeamConsoleVariables::DrawBeamTracesDuration > 0.0f)
	{
		if (SweepRadius > 0.0f)
		{
			const FVector SweepDirection = (TraceEndLocation - TraceStartLocation).GetSafeNormal();
			const float CapsuleLength = (TraceEndLocation - TraceStartLocation).Size() + 2 * SweepRadius; // Add diameter to cover both ends
			const FQuat CapsuleRotation = FRotationMatrix::MakeFromZ(SweepDirection).ToQuat();
			DrawDebugCapsule(GetWorld(), TraceStartLocation + SweepDirection * (CapsuleLength * 0.5f - SweepRadius), CapsuleLength * 0.5f, SweepRadius, CapsuleRotation, FColor::Blue, false, SunBeamConsoleVariables::DrawBeamTracesDuration);
		}
		else
		{
			static float DebugThickness = 1.0f;
			DrawDebugLine(GetWorld(), TraceStartLocation, TraceEndLocation, FColor::Green, false, SunBeamConsoleVariables::DrawBeamTracesDuration, 0, DebugThickness);
		}

		static float DebugPointSize = 5.0f;
		DrawDebugPoint(GetWorld(), TraceEndLocation, DebugPointSize, FColor::Blue, false, SunBeamConsoleVariables::DrawBeamTracesDuration);
	}
#endif // ENABLE_DRAW_DEBUG

	SetBeamEndLocation(TraceEndLocation);
	return bHit;
}

void ABeamActor::SetBeamEndLocation(const FVector& EndLocation) const
{
	BeamEffectComponent->SetVariableVec3(FName("Beam_end"), EndLocation);
	const float ZSize = FVector::Distance(GetActorLocation(), EndLocation) * 0.0051;
	BeamEffectComponent->SetVariableVec3(FName("BeamScale"), FVector(1, 1, ZSize));

	// TODO: Temp: Set player beam end pos to material collection
	if (Cast<ABeamPawn>(GetBeamOwner()))
	{
		ALightTrackerObj::GetInstance(GetWorld())->SetMaterialVectorParameter(FName("LightPos"), FLinearColor(EndLocation));
	}
}

void ABeamActor::SetBeamOwner(AActor* InBeamOwner)
{
	// check if the beam owner is valid and implements the IBeamSpawner interface
	check(InBeamOwner);
	check(InBeamOwner->GetClass()->ImplementsInterface(UBeamSpawner::StaticClass()));
	BeamOwner = InBeamOwner;
}

void ABeamActor::SetBeamSourceTag(const FGameplayTag& InBeamSourceTag)
{
	LightSourceTag = InBeamSourceTag;
}

void ABeamActor::SetBeamActiveStatus(const bool bIsActive)
{
	SetActorTickEnabled(bIsActive);
	SetActorHiddenInGame(not bIsActive);

	if (not bIsActive)
	{
		for (AActor* LastBeamHitInteractable : LastBeamHitInteractables)
		{
			IInteractable::Execute_OnEndInteract(LastBeamHitInteractable);
		}

		LastBeamHitInteractables.Empty();
	}

	ALightTrackerObj::GetInstance(GetWorld())->AddBeamCount(bIsActive ? 1 : -1);
}

FGameplayTag ABeamActor::GetLightSourceTag_Implementation() const
{
	return LightSourceTag;
}

bool ABeamActor::CanInteractWithActor_Implementation(AActor* OtherActor) const
{
	if (!IsValid(OtherActor) or !OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return false;
	}

	// Check if the beam source tag is in the interactable tags
	check(LightSourceTag != FGameplayTag::EmptyTag);
	const IInteractable* Interactable = Cast<IInteractable>(OtherActor);
	FGameplayTagContainer InteractableTags;
	Interactable->Execute_GetInteractableTags(OtherActor, InteractableTags);
	return InteractableTags.HasTag(LightSourceTag);
}
