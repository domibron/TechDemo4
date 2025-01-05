// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomPlayerController.h"

#include "GameFramework/Character.h"

void AMyCustomPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if (MainCharacter == nullptr)
	// {
	// 	MainCharacter = Cast<AMainCharacter>(GetCharacter());
	// }
	//
	// if (MainCharacter)
	// {
		// if (MainCharacter->PlayerID == 1 && MainCharacter->bAiming)
		// {
		// 	SetDeprecatedInputPitchScale(.5);
		// 	SetDeprecatedInputYawScale(.5);
		// 	GLog->Log("ads");
		// }
		// else if (MainCharacter->PlayerID == 1 && !MainCharacter->bAiming)
		// {
		// 	SetDeprecatedInputPitchScale(2.5);
		// 	SetDeprecatedInputYawScale(2.5);
		// 	GLog->Log("no ads");
		//
		// }
	// }
}

void AMyCustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// if (MainCharacter == nullptr)
	// {
	// 	MainCharacter = Cast<AMainCharacter>(GetCharacter());
	// }
}

AMyCustomPlayerController::AMyCustomPlayerController()
{
	// PrimaryActorTick.bCanEverTick = true;
}
