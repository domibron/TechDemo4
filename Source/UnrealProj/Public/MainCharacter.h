// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainGameMode.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PlayerUIUSerWidget.h"

	#include "NiagaraFunctionLibrary.h"
	#include "NiagaraComponent.h"

#include "Materials/Material.h"

#include "MainGameMode.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"

// keep here otherwise it will break
#include "MainCharacter.generated.h"


UCLASS()
class UNREALPROJ_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()
	
//vars
public:
	// Sets default values for this character's properties
	AMainCharacter();
	
	float CurrentHealth;
	
	UPROPERTY(EditAnywhere)
	float MaxHealth = 200.0f;

	
	UPROPERTY(EditAnywhere)
	int MaxClipSize = 10;

	UPROPERTY(EditAnywhere)
	float ReloadDuration = 2.0f;

	UPROPERTY(EditAnywhere)
	float DamageIndicatorDuration = 2.0;

	// aiming
	bool bAiming = false;

	// player ID
	int PlayerID = -1;

	
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	UMaterial* PlayerOneMat;

	UPROPERTY(EditAnywhere)
	UMaterial* PlayerTwoMat;

	bool bReloading = false;
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* LaserStartMesh;
	
	float DotProductCamAngle;

	static float DotProductFVector(FVector v1, FVector v2);
	
private:

	
	// resfs.
	UNiagaraComponent* NiagaraSystem;
	UPlayerUIUSerWidget* PlayerUI;
	AMainGameMode* MainGameMode;
	USpringArmComponent* CameraBoom;
		
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere)
	USoundBase* ShootSFX;
	UPROPERTY(EditAnywhere)
	USoundBase* DeathSFX;

	float DamageIndicatorTimeRemaining = 0.0f;
	
	bool bPLayerHasUI = false;


	float ADSLerpTime = 0.0f;
	
	// weapon stats
	float currentFireTimeLeft = 0.0;
	UPROPERTY(EditAnywhere)
	float fireRate = 4.0;

	// weapon damage
	UPROPERTY(EditAnywhere)
	float bodyDamage = 10.0f;
	UPROPERTY(EditAnywhere)
	float headDamage = 50.0f;

	
	bool bLaserActive = true;


	int AmmoPool = 30;
	int AmmoInClip = 10;

	float ReloadTimeLeft = 0.0f;
	

	float DamageMultiplier = 1.0;
	float DamageMultiplierDurationRemainder = 0.0f;
	
	bool bSetMat = false;

	
// ======= functions =======
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// extern call for getting hit.
	void TakeDamage(float DamageAmount);
	void Heal(float HealAmmount);
	void GiveAmmo(int Ammount);
	
	void GiveDamageBuff(float Mult, float Duration);

	static float LerpFloat(float a, float b, float t);

private:

	void ReloadAmmo();

	// input functions
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookRight(float Value);
	void LookUp(float Value);

	void Fire();
	void Aim();
	void UnAim();
	void BeginReload();

	void Reload();
	
	void JumpPressed();

	void QuitToMainMenu();
};
