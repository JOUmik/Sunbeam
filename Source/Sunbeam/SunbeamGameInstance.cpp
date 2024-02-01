// Fill out your copyright notice in the Description page of Project Settings.


#include "SunbeamGameInstance.h"
#include "Kismet/GameplayStatics.h"

void USunbeamGameInstance::ChangeMap(int index)
{
	if(index == CurrentMapIndex) return;
	else
	{
		if(index == 2)
		{
			CurrentMapIndex = 2;
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Map_Level_1"));
		}
		else
		{
			CurrentMapIndex = 1;
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("Map_Level_2"));
		}
	}
}
