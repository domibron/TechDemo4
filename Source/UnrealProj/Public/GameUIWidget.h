// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJ_API UGameUIWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadWrite)
	FString GameTimeAsString = "999";

	UPROPERTY(BlueprintReadWrite)
	FString PlayerOneScore = "0";
	
	UPROPERTY(BlueprintReadWrite)
	FString PlayerTwoScore = "0";
};
