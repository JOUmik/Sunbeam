// Copyright Project SunBeam. All Rights Reserved.


#include "Actor/BeamObjective.h"

#include "Game/BeamGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABeamObjective::ABeamObjective()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABeamObjective::OnBeginInteract_Implementation(FHitResult LightHitResult, AActor* LightSource)
{
	ABeamGameModeBase* BeamGameMode = Cast<ABeamGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (BeamGameMode->IsObjectiveCompleted())
	{
		return;
	}
	
	if (LightSourceCount == 0)
	{
		PlayBloomAnimForward();
	}
	LightSourceCount++;
}

void ABeamObjective::OnEndInteract_Implementation()
{
	ABeamGameModeBase* BeamGameMode = Cast<ABeamGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (BeamGameMode->IsObjectiveCompleted())
	{
		return;
	}
	
	LightSourceCount--;
	if (LightSourceCount == 0)
	{
		PlayBloomAnimReverse();
	}
}

void ABeamObjective::GetInteractableResponseTags_Implementation(FGameplayTagContainer& OutTagContainer)
{
	OutTagContainer = InteractableResponseTags;
}

void ABeamObjective::SetBloomStatus(const bool bBloomed)
{
	OnObjectiveStateChangedDelegate.Broadcast(bBloomed);
}

void ABeamObjective::BeginPlay()
{
	Super::BeginPlay();
}

