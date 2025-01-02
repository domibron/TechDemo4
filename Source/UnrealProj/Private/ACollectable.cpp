// Fill out your copyright notice in the Description page of Project Settings.


#include "ACollectable.h"

#include "ACollectableSpawner.h"


// Sets default values
AACollectable::AACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	
	
}

void AACollectable::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StaticMesh = FindComponentByClass<UStaticMeshComponent>();
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AACollectable::CollidedWith);
}


void AACollectable::SetVisible(bool bVisible)
{
	SetActorHiddenInGame(!bVisible);
	SetActorEnableCollision(bVisible);
	SetActorTickEnabled(bVisible);
	// StaticMesh->SetHiddenInGame(!bVisible, true);
}

// Called when the game starts or when spawned
void AACollectable::BeginPlay()
{
	Super::BeginPlay();

	SetUp();
	
}

// Called every frame
void AACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AACollectable::SetUp()
{
	CollectableType = static_cast<ECollectableType>(FMath::RandRange(0, 2));
	
	if (CollectableType == ECollectableType::Heal)
	{
		StaticMesh->SetMaterial(0, Redmat);
	}
	else if (CollectableType == ECollectableType::Ammo)
	{
		StaticMesh->SetMaterial(0, Greenmat);

	}
	else if (CollectableType == ECollectableType::DamageMult)
	{
		StaticMesh->SetMaterial(0, BlueMat);

	}


	bCollected = false;

	// call the function pointer;
	// if (CollectFuntionCall) this->*CollectFuntionCall;
}

void AACollectable::CollidedWith(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCollected) return;
	
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	
	
	
	if (MainCharacter)
	{
		if (CollectableType == ECollectableType::Heal)
		{
			Heal(MainCharacter);
		}
		else if (CollectableType == ECollectableType::Ammo)
		{
			Ammo(MainCharacter);

		}
		else if (CollectableType == ECollectableType::DamageMult)
		{
			DamageMult(MainCharacter);

		}

		bCollected = true;
		
		if (Spawner) Spawner->ReturnActorToPool(this);
		
		// call function to collect and remove from scene to go back in the pool.
		
	}
}

void AACollectable::Heal(AMainCharacter* Player)
{
	Player->Heal(HealGiveAmmount);

	// destroy this or return to pool.
}

void AACollectable::Ammo(AMainCharacter* Player)
{
	Player->GiveAmmo(AmmoGiveAmmount);
}

void AACollectable::DamageMult(AMainCharacter* Player)
{
	Player->GiveDamageBuff(DamageMultiplier, DamageMultiplierDuration);
}



