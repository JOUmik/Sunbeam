// Copyright Project SunBeam. All Rights Reserved.


#include "Actor/BeamFlowerBase.h"

// Sets default values
ABeamFlowerBase::ABeamFlowerBase()
{
	PrimaryActorTick.bCanEverTick = false;

}
void ABeamFlowerBase::GetInteractableTags_Implementation(FGameplayTagContainer& OutTagContainer)
{
	OutTagContainer = InteractableTags;
}

// Called when the game starts or when spawned
void ABeamFlowerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABeamFlowerBase::SetBloomStatus(bool bBloomed)
{
	bHasBloomed = bBloomed;

	if(bHasBloomed)
	{
		// TODO: Subtract energy from the player
	} else
	{
		// TODO: Add energy to the player
	}
}

