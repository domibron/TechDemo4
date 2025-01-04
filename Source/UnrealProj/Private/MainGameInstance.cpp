// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"

void UMainGameInstance::OnStart()
{
	Super::OnStart();
}

APlayerController* UMainGameInstance::GetPlayerWithID(int id)
{
	return GetLocalPlayerByIndex(id)->GetPlayerController(GetWorld());
}
