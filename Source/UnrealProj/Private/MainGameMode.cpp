// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"




AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (GlobalUIAsset)
	{
		GlobalUIWidget = CreateWidget<UGameUIWidget>(GetWorld(), GlobalUIAsset);
	}
if (GlobalUIWidget)
{
	GlobalUIWidget->AddToViewport();
	GlobalUIWidget->SetVisibility(ESlateVisibility::Visible);
}
	
	CurrentTime = DefultTime;
}

void AMainGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CurrentTime -= DeltaSeconds;

	
}


