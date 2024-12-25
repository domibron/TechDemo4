// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

	#include "NiagaraFunctionLibrary.h"
	#include "NiagaraComponent.h"

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
	UNiagaraComponent* NiagaraSystem;
	
	float currentFireTimeLeft = 0.0;
	UPROPERTY(EditAnywhere)
	float fireRate = 4.0;
	
	UPROPERTY(EditAnywhere)
float bodyDamage = 10.0f;
	UPROPERTY(EditAnywhere)
	float headDamage = 50.0f;

	bool bAiming = false;
	bool bLaserActive = true;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookRight(float Value);
	void LookUp(float Value);

	void Fire();
	void Aim();
	void UnAim();
	
	void JumpPressed();
};
