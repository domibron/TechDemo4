// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

#include "MainMenu.generated.h"


/**
 * 
 */
UCLASS()
class UNREALPROJ_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName GameLevelName = "LEVEL NAME HERE";
	
	UFUNCTION(BlueprintCallable)
	void QuitGame();

	UFUNCTION(BlueprintCallable)
	void StartGame();
	
};
