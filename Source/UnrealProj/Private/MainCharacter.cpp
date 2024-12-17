// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"

#include "Camera/CameraComponent.h"


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

	CurrentHealth = MaxHealth;
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (AutoReceiveInput == 1) GLog->Log("Player1");
	if (AutoReceiveInput == 2) GLog->Log("Player2");
	//GLog->Log(FString::FromInt(AutoReceiveInput));
	
}
// tesdt
// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	
		PlayerInputComponent->BindAxis("XKeys", this, &AMainCharacter::MoveForward);
		PlayerInputComponent->BindAxis("YKeys", this, &AMainCharacter::MoveRight);
	
		PlayerInputComponent->BindAction("JumpKey", IE_Pressed, this, &AMainCharacter::JumpPressed);
	
		PlayerInputComponent->BindAxis("Mouse X", this, &AMainCharacter::LookRight);
		PlayerInputComponent->BindAxis("Mouse Y", this, &AMainCharacter::LookUp);
	
		PlayerInputComponent->BindAction("FireKey", IE_Pressed, this, &AMainCharacter::Fire);
		PlayerInputComponent->BindAction("AimKey", IE_Pressed, this, &AMainCharacter::Aim);


	
		PlayerInputComponent->BindAxis("XStick", this, &AMainCharacter::MoveForward);
		PlayerInputComponent->BindAxis("YStick", this, &AMainCharacter::MoveRight);

		PlayerInputComponent->BindAction("JumpButton", IE_Pressed, this, &AMainCharacter::JumpPressed);

		PlayerInputComponent->BindAxis("XLookStick", this, &AMainCharacter::LookRight);
		PlayerInputComponent->BindAxis("YLookStick", this, &AMainCharacter::LookUp);

		PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AMainCharacter::Fire);
		PlayerInputComponent->BindAction("AimButton", IE_Pressed, this, &AMainCharacter::Aim);
	
	

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

void AMainCharacter::Fire()
{
	FHitResult Hit;

	UCameraComponent* CameraComponent = FindComponentByClass<UCameraComponent>();
	
	FVector StartLocation = CameraComponent->GetComponentLocation();

	FVector EndLocation = (CameraComponent->GetForwardVector() * 200)+StartLocation;

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();
	

	bool bHasHit = ActorLineTraceSingle(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_PhysicsBody, CollisionParams);

	
}

void AMainCharacter::Aim()
{
}

void AMainCharacter::JumpPressed()
{
	Jump();
}


