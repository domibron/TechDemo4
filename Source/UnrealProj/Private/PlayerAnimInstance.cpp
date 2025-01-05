// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn != nullptr && PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AMainCharacter>(GetOwningActor());
	}
		
	
}

void UPlayerAnimInstance::CustomUpdateAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AMainCharacter>(GetOwningActor());
	}
	
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		MoveSpeed = LateralSpeed.Size();
		
		
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		
		
		if (PlayerCharacter)
		{
			MoveDirection = Pawn->GetActorRotation().UnrotateVector(Pawn->GetLastMovementInputVector());
			
		}
	}
}
