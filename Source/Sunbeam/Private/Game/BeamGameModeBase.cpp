// Project SunBeam, all rights reserved.


#include "Game/BeamGameModeBase.h"

#include "Actor/BeamObjective.h"
#include "Interface/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Singleton/BeamGameplayTags.h"

void ABeamGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	BufferInteractableByType();
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
			FGameplayTagContainer InteractableResponseTags;
			IInteractable::Execute_GetInteractableResponseTags(Interactable, InteractableResponseTags);

			FBeamGameplayTags BeamGameplayTags = FBeamGameplayTags::Get();
			for (auto& Tag : InteractableResponseTags)
            {
				if (Tag.MatchesTag(BeamGameplayTags.LightSourceBase))
				{
					InteractableMap.FindOrAdd(Tag).Add(Interactable);
				}
            }
			
			if (ABeamObjective* Objective = Cast<ABeamObjective>(Interactable))
			{
				Objective->OnObjectiveStateChangedDelegate.AddDynamic(this, &ABeamGameModeBase::OnObjectiveStateChanged);
				ObjectiveCount++;
			}
		}
	}
}

void ABeamGameModeBase::ShowInteractableByType(const FGameplayTag& InteractableType) const
{
	// 1. Set hidden for all interactables
	for (const auto& InteractableArray : InteractableMap)
	{
		for (AActor* Interactable : InteractableArray.Value)
		{
			Interactable->SetActorHiddenInGame(true);
		}
	}

	// 2. Set visible for interactables with the given type
	if (const TArray<AActor*>* InteractableArray = InteractableMap.Find(InteractableType))
	{
		for (AActor* Interactable : *InteractableArray)
		{
			Interactable->SetActorHiddenInGame(false);
		}
	}
}

int32 ABeamGameModeBase::GetObjectiveCount() const
{
	return ObjectiveCount;
}

int32 ABeamGameModeBase::GetObjectiveCompletedCount() const
{
	return ObjectiveCompletedCount;
}

void ABeamGameModeBase::OnObjectiveStateChanged(const bool bNewState)
{
	ObjectiveCompletedCount += bNewState ? 1 : -1;
	OnObjectiveCompletedCountChangeDelegate.Broadcast(ObjectiveCompletedCount, ObjectiveCount);
	
	if (ObjectiveCompletedCount == ObjectiveCount)
	{
		// All objectives are completed
		// TODO: Winning Condition
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("All objectives are completed"));
	}
}
