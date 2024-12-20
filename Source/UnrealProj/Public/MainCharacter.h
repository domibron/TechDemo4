// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

// keep here otherwise it will break
#include "MainCharacter.generated.h"


UCLASS()
class UNREALPROJ_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	float CurrentHealth;
	float MaxHealth = 200.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TakeDamage(float DamageAmount);

private:
	UCameraComponent* CameraComponent;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookRight(float Value);
	void LookUp(float Value);

	void Fire();
	void Aim();
	
	void JumpPressed();
};
