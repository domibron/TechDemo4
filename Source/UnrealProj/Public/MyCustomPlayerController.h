// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharacter.h"

#include "MyCustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJ_API AMyCustomPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;
	
	virtual void BeginPlay() override;

	// AMainCharacter* MainCharacter;

	AMyCustomPlayerController();
	
};
