// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacter.h"

#include "PlayerAnimInstance.generated.h"

/**
 * a
 */
UCLASS()
class UNREALPROJ_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "CustomAnim")
	void CustomUpdateAnimation();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAnim")
	bool bIsInAir;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAnim")
	float MoveSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAnim")
	class APawn* Pawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAnim")
	FVector MoveDirection;

	AMainCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAnim")
	float SpineXRotation = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAnim")
	bool bFiring = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAnim")
	bool bIsAiming = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CustomAnim")
	bool bIsReloading = false;

	
};
