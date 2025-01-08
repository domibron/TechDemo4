// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"

#include "MainCharacter.h"


AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	MainGameInstance = Cast<UMainGameInstance>(GetGameInstance());

	if (GlobalUIAsset)
	{
		GlobalUIWidget = CreateWidget<UGameUIWidget>(GetWorld(), GlobalUIAsset);
	}
if (GlobalUIWidget)
{
	GlobalUIWidget->AddToViewport();
	GlobalUIWidget->SetVisibility(ESlateVisibility::Visible);

	GlobalUIWidget->PlayerOneScore = FString::FromInt(MainGameInstance->PlayerOneScore);
	GlobalUIWidget->PlayerTwoScore = FString::FromInt(MainGameInstance->PlayerTwoScore);
	
}
	
	CurrentTime = DefultTime;
}

void AMainGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (timer > 0) timer -= DeltaSeconds;

	if (timer <= 0 && bLoadLevel)
	{
		
		LoadLevel(NameOfLevelToLoad);
	}
	
	if (CurrentTime > 0 && !bGameEnded) CurrentTime -= DeltaSeconds;

	if(GlobalUIWidget)
	{
		GlobalUIWidget->GameTimeAsString = FString::Printf(TEXT("%.f"), CurrentTime);
	}

	if (CurrentTime <= 0 && !bGameEnded)
	{
		// game ended.
		bGameEnded = true;

		int winner = -999;
		if (MainGameInstance)
		{
			AMainCharacter* PlayerOne = Cast<AMainCharacter>((MainGameInstance->GetPlayerWithID(0))->GetCharacter());
			AMainCharacter* PlayerTwo = Cast<AMainCharacter>((MainGameInstance->GetPlayerWithID(1))->GetCharacter());

			if (PlayerOne && PlayerTwo)
			{
				if (PlayerOne->CurrentHealth > PlayerTwo->CurrentHealth) winner = 0;
				else if (PlayerOne->CurrentHealth < PlayerTwo->CurrentHealth) winner = 1;
			}
		}
		
		
		OnGameEnd(winner);
	}
	
}

void AMainGameMode::PlayerDied(int PlayerID)
{
	GLog->Log("Round Ended " + FString::FromInt(PlayerID));
	bGameEnded = true;

	if (PlayerID == 0)
		OnGameEnd(1);
	else if (PlayerID == 1)
		OnGameEnd(0);
}

void AMainGameMode::OnGameEnd(int WinnerID)
{
	
	if (WinnerID == 0)
		MainGameInstance->PlayerOneScore++;
	else if (WinnerID == 1)
		MainGameInstance->PlayerTwoScore++;

	MainGameInstance->CurrentRound++;
	
	if (MainGameInstance->CurrentRound < 5)
	{
		// we play again.
		timer = DelayLevelLoadTime;

		if(GlobalUIWidget)
		{
			GlobalUIWidget->WinnerColor = FColor::White;
				
			if (WinnerID == 0)
				GlobalUIWidget->WinnereText = "Player left wins the round";
			else if (WinnerID == 1)
				GlobalUIWidget->WinnereText = "Player right wins the round";
			else
				GlobalUIWidget->WinnereText = "No one wins the round";
		}
			
			
		NameOfLevelToLoad = FName(*(UGameplayStatics::GetCurrentLevelName(GetWorld(), true)));
		bLoadLevel = true;
	}
	else
	{
		if (MainGameInstance->PlayerOneScore > MainGameInstance->PlayerTwoScore)
		{
			timer = DelayLevelLoadTime;
		
			// player one wins.
			if(GlobalUIWidget)
			{
				GlobalUIWidget->WinnerColor = FColor::White;
						
				GlobalUIWidget->WinnereText = "Player left wins";
			}

			//MainGameInstance->GetPlayerWithID(1)->Destroy();
			//MainGameInstance->GetPlayerWithID(0)->bShowMouseCursor = true;
		
			NameOfLevelToLoad = FName("MainMenu");
			bLoadLevel = true;
		}
		else if (MainGameInstance->PlayerOneScore < MainGameInstance->PlayerTwoScore)
		{
			timer = DelayLevelLoadTime;
		
			if(GlobalUIWidget)
			{
				GlobalUIWidget->WinnerColor = FColor::White;
			
				GlobalUIWidget->WinnereText = "Player right wins";
			}

			MainGameInstance->GetPlayerWithID(1)->Destroy();
			MainGameInstance->GetPlayerWithID(0)->bShowMouseCursor = true;
		
			NameOfLevelToLoad = FName("MainMenu");
			bLoadLevel = true;
		}
		else
		{
			timer = DelayLevelLoadTime;
		
			if(GlobalUIWidget)
			{
				GlobalUIWidget->WinnerColor = FColor::White;
			
				GlobalUIWidget->WinnereText = "No one wins";
			}

			MainGameInstance->GetPlayerWithID(1)->Destroy();
			MainGameInstance->GetPlayerWithID(0)->bShowMouseCursor = true;
		
			NameOfLevelToLoad = FName("MainMenu");
			bLoadLevel = true;
		}
	}
}

void AMainGameMode::LoadLevel(FName LevelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}


