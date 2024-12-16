// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
// tesdt
// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("XKeys", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("YKeys", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("JumpKey", IE_Pressed, this, &AMainCharacter::Jump);

	PlayerInputComponent->BindAxis("Mouse X", this, &AMainCharacter::LookRight);
	PlayerInputComponent->BindAxis("Mouse Y", this, &AMainCharacter::LookUp);
}

void AMainCharacter::MoveForward(float Value)
{
	AMainCharacter::AddMovementInput(AMainCharacter::GetActorForwardVector(), Value, true);
}

void AMainCharacter::MoveRight(float Value)
{
	AMainCharacter::AddMovementInput(AMainCharacter::GetActorRightVector(), Value, true);
}

void AMainCharacter::LookRight(float Value)
{
	// FRotator rotation = GetActorRotation();
	//
	// rotation += FRotator(0, Value, 0);
	//
	// SetActorRotation(rotation);

	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * -1.0f);
}

void AMainCharacter::Jump()
{
	Super::Jump();
}


