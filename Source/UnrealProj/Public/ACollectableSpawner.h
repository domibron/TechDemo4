// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACollectable.h"
#include "GameFramework/Actor.h"
#include "ACollectableSpawner.generated.h"



UCLASS()
class UNREALPROJ_API AACollectableSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACollectableSpawner();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AACollectable> CollectableToSpawn;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> PossibleSpawnLocations;
	
	void ReturnActorToPool(AActor* ActorToReturn);

	float PickUpSpawnTime = 15;

	// KEEP A VALUE ABOVE 1, otherwise it will try to access collectables that dont exsist.
	float SpawnTime = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<AACollectable*> CollectablePool;

	void TryToSpawnCollectable();

	FVector GetSpawnLocation(const AACollectable* CollectableToIgnore);
};
