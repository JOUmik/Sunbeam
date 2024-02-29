// Project SunBeam, all rights reserved.


#include "Game/BeamGameModeBase.h"

#include "Interface/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

void ABeamGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	BufferInteractableByType();
	check(DefaultShowingInteractableType.IsValid());
}

void ABeamGameModeBase::AddBeamCount(const int32 Count)
{
	BeamCount += Count;
	BeamCount = FMath::Max(BeamCount, 0);
	
	SetMaterialScalarParameter("BeamCount", BeamCount);
}

void ABeamGameModeBase::AddSecondaryLightCount(const int32 Count)
{
	SecondaryLightCount += Count;
	SecondaryLightCount = FMath::Max(SecondaryLightCount, 0);
	
	SetMaterialScalarParameter("PointLightCount", SecondaryLightCount);
}

void ABeamGameModeBase::SetMaterialScalarParameter(const FName ParameterName, const float Value) const
{
	if (!IsValid(GetWorld()))
	{
		return;
	}
	if (const UMaterialParameterCollection* Collection = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/Materials/Interact/GetLightCount")))
	{
		if (UMaterialParameterCollectionInstance* CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection))
		{
			CollectionInstance->SetScalarParameterValue(ParameterName, Value);
		}
	}
}

void ABeamGameModeBase::SetMaterialVectorParameter(const FName ParameterName, const FLinearColor& Value) const
{
	if (!IsValid(GetWorld()))
	{
		return;
	}
	if (const UMaterialParameterCollection* Collection = LoadObject<UMaterialParameterCollection>(nullptr, TEXT("/Game/Materials/Interact/GetLightCount")))
	{
		if (UMaterialParameterCollectionInstance* CollectionInstance = GetWorld()->GetParameterCollectionInstance(Collection))
		{
			CollectionInstance->SetVectorParameterValue(ParameterName, Value);
		}
	}
}

void ABeamGameModeBase::BufferInteractableByType()
{
	// Find all interactables in game
	TArray<AActor*> Interactables;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UInteractable::StaticClass(), Interactables);

	for (AActor* Interactable : Interactables)
	{
		if (Interactable->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			FGameplayTag InteractableTag;
			IInteractable::Execute_GetInteractableAssetTag(Interactable, InteractableTag);
			if (InteractableTag != FGameplayTag::EmptyTag)
			{
				InteractableMap.FindOrAdd(InteractableTag).Add(Interactable);

				// Add the children of the interactable to the map
				TArray<AActor*> ChildActors;
				Interactable->GetAttachedActors(ChildActors);
				for (AActor* ChildActor : ChildActors)
				{
					InteractableMap.FindOrAdd(InteractableTag).Add(ChildActor);
				}
			}
		}
	}
}

void ABeamGameModeBase::ShowInteractableByType(const FGameplayTag& InteractableType) const
{
	for (const auto& InteractablePair : InteractableMap)
	{
		const bool bShow = (InteractablePair.Key == InteractableType);
		for (AActor* Interactable : InteractablePair.Value)
		{
			Interactable->SetActorHiddenInGame(!bShow);
			Interactable->SetActorTickEnabled(bShow);
			Interactable->SetActorEnableCollision(bShow);
		}
	}
}
