// Fill out your copyright notice in the Description page of Project Settings.


#include "ACollectableSpawner.h"

// Sets default values
AACollectableSpawner::AACollectableSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AACollectableSpawner::ReturnActorToPool(AActor* ActorToReturn)
{
	for (int i = 0; i < CollectablePool.Num(); i++)
	{
		if (CollectablePool[i] == ActorToReturn)
		{
			CollectablePool[i]->SetVisible(false);
			CollectablePool[i]->bSpawned = false;
		}
	}
}

// Called when the game starts or when spawned
void AACollectableSpawner::BeginPlay()
{
	Super::BeginPlay();

	
	
	CollectablePool.Init(nullptr, 4);
	CollectablePool.Empty();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Owner = this;
	spawnParameters.Instigator= GetInstigator();
	
	for (int i = 0; i < 4; i++)
	{
		AACollectable* collectableSpawned = GetWorld()->SpawnActor<AACollectable>(CollectableToSpawn, FVector(0,0,0), FRotator(0,0,0), spawnParameters);
		CollectablePool.Emplace(collectableSpawned);
		CollectablePool[i]->SetVisible(false);
		CollectablePool[i]->Spawner = this;
	}
}

// Called every frame
void AACollectableSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(SpawnTime > 0)
	{
		SpawnTime -= DeltaTime;
	}
	else
	{
		TryToSpawnCollectable();
	}
	
	
	
}

void AACollectableSpawner::TryToSpawnCollectable()
{
	if (PossibleSpawnLocations.Num() > 0 && CollectablePool.Num() > 0)
	{
		for (int i = 0; i < CollectablePool.Num(); i++)
		{
			if (!CollectablePool[i]->bSpawned)
			{
				FVector locationToMoveTo = GetSpawnLocation(CollectablePool[i]);
				if (locationToMoveTo != FVector(-999,-999,-999))
				{
					CollectablePool[i]->SetActorLocation(locationToMoveTo);
					CollectablePool[i]->SetVisible(true);
					CollectablePool[i]->bSpawned = true;
				}
				break;
			}
		}
	}
	else
	{
		GLog->Log("Failed to spawn collectalbe");
	}

	SpawnTime = PickUpSpawnTime;
}

FVector AACollectableSpawner::GetSpawnLocation(const AACollectable* CollectableToIgnore)
{
	TArray<AActor*> spawnLocations;

	//FVector targetLocation = PossibleSpawnLocations[FMath::RandRange(0, PossibleSpawnLocations.Num())]->GetActorLocation();
			
	for (int i = 0; i < PossibleSpawnLocations.Num(); i++)
	{
		bool availableLocation = true;

		for (int a = 0; a < CollectablePool.Num(); a++)
		{
			if (CollectablePool[a] != CollectableToIgnore)
			{
				if (CollectablePool[a]->GetActorLocation() == PossibleSpawnLocations[i]->GetActorLocation())
				{
					availableLocation = false;
				}
			}
		}

		if (availableLocation)
		{
			spawnLocations.Emplace(PossibleSpawnLocations[i]);
		}
	}

	if (spawnLocations.Num() > 0)
	{
		return spawnLocations[FMath::RandRange(0, spawnLocations.Num() - 1)]->GetActorLocation();
	}
	else
	{
		return FVector(-999,-999,-999);
	}
}

