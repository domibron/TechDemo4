// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

#include "Kismet/GameplayStatics.h"

void UMainGameInstance::OnStart()
{
	Super::OnStart();
}

APlayerController* UMainGameInstance::GetPlayerWithID(int id)
{
	return GetLocalPlayerByIndex(id)->GetPlayerController(GetWorld());
}

void UMainGameInstance::StartGame(FName LevelName)
{
	PlayerOneScore = 0;
	PlayerTwoScore = 0;
	
	
	UGameplayStatics::OpenLevel(GetWorld(), LevelName, true);
}
