// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUIUSerWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPROJ_API UPlayerUIUSerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float PlayerOneHealthPercentage = 1.0;

	// UPROPERTY(BlueprintReadWrite)
	// float PlayerTwoHealthPercentage = 1.0;

	UPROPERTY(BlueprintReadWrite)
	FString PlayerOneWeaponString = TEXT("WEAPON P1");

	// UPROPERTY(BlueprintReadWrite)
	// FString PlayerTwoWeaponString = TEXT("WEAPON P2");
	
	UPROPERTY(BlueprintReadWrite)
	FString PlayerOneHealthText = TEXT("200");

	// UPROPERTY(BlueprintReadWrite)
	// FString PlayerTwoHealthText = TEXT("200");

	UPROPERTY(BlueprintReadWrite)
	FLinearColor DamageColor = FLinearColor(1.0,1.0,1.0,0.0);

	float DamageTrancparancyAmmount = 0.0;

	float DamageMaxTransparancyAmmount = 0.3;

	UPROPERTY(BlueprintReadWrite)
	FLinearColor DamageMultColor = FLinearColor(1.0,1.0,1.0,0.0);

	float DamageMultTrancparancyAmmount = 0.0;

	UPROPERTY(BlueprintReadWrite)
	float CurrentTime = 300.0f;

	int PlayerOneWins = 0;

	int PlayerTwoWins = 0;

	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	FLinearColor HitMarkerColor = FLinearColor(1.0,1.0,1.0,0.0);
};
