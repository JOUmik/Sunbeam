// Copyright Project SunBeam. All Rights Reserved.


#include "Singleton/LightTrackerObj.h"

#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

ALightTrackerObj* ALightTrackerObj::GetInstance(UWorld* World)
{
	if (!Instance || !IsValid(Instance))
	{
		// Ensure we have a valid world context before spawning
		if (World)
		{
			// Spawn the singleton actor into the world
			Instance = World->SpawnActor<ALightTrackerObj>();
		}
	}
	return Instance;
}

void ALightTrackerObj::AddBeamCount(const int32 Count)
{
	BeamCount += Count;
	BeamCount = FMath::Max(BeamCount, 0);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Beam Count: %d"), BeamCount));
	SetMaterialScalarParameter("BeamCount", BeamCount);
}

void ALightTrackerObj::AddSecondaryLightCount(const int32 Count)
{
	SecondaryLightCount += Count;
	SecondaryLightCount = FMath::Max(SecondaryLightCount, 0);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Secondary Light Count: %d"), SecondaryLightCount));
	SetMaterialScalarParameter("PointLightCount", SecondaryLightCount);
}

void ALightTrackerObj::SetMaterialScalarParameter(const FName ParameterName, const float Value) const
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

void ALightTrackerObj::SetMaterialVectorParameter(FName ParameterName, const FLinearColor& Value) const
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

void ALightTrackerObj::BeginDestroy()
{
	Instance = nullptr;
	Super::BeginDestroy();
}
