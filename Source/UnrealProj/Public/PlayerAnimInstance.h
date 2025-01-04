// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


#include "PlayerAnimInstance.generated.h"

/**
 * 
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
	
};
