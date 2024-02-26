// Copyright Project SunBeam. All Rights Reserved.


#include "SunbeamGameInstance.h"
#include "Kismet/GameplayStatics.h"

void USunbeamGameInstance::ChangeMap(int index)
{
	if(index == CurrentMapIndex) return;
	else
	{
		if(index == 1)
		{
			CurrentMapIndex = 1;
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Map_Level_1"));
		}
		else if(index == 2)
		{
			CurrentMapIndex = 2;
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Map_Level_2"));
		}
		else
		{
			CurrentMapIndex = 0;
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestMap"));
		}
	}
}
