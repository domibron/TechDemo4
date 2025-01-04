// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "GameUIWidget.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"


#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJ_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMainGameMode();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Widgets")
	TSubclassOf<class UGameUIWidget> GlobalUIAsset;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Widgets")
	UGameUIWidget* GlobalUIWidget;

	UMainGameInstance* MainGameInstance;
	
	UPROPERTY(EditAnywhere)
	float DefultTime = 300.0f;
	
	float CurrentTime = 300.0f;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;;

	void PlayerDied(int PlayerID);

	bool bGameEnded = false;

	void OnGameEnd(int WinnerID);

	UPROPERTY(EditAnywhere)
	float DelayLevelLoadTime = 2;
	
	float timer = 0;

	bool bLoadLevel = false;

	void LoadLevel(FName LevelName);

	FName NameOfLevelToLoad;
};
