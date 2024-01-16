// Copyright Epic Games, Inc. All Rights Reserved.

#include "SunbeamGameMode.h"
#include "SunbeamCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASunbeamGameMode::ASunbeamGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
