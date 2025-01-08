// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJ_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void OnStart() override;

	APlayerController* GetPlayerWithID(int id);

	int PlayerOneScore = 0;
	int PlayerTwoScore = 0;

	int CurrentRound = 0;
	
	void StartGame(FName LevelName);
};
