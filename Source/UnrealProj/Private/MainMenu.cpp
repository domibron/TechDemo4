// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"



void UMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

void UMainMenu::StartGame()
{
	if (UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		MainGameInstance->StartGame(GameLevelName);
	}
}
