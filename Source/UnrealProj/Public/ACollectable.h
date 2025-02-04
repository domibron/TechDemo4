// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MainCharacter.h"


#include "Components/AudioComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACollectable.generated.h"

// forward declaration. prevent the compiler from entering a endless loop. We tell it to look at this class first, then this.
class AACollectableSpawner;

UENUM()
enum ECollectableType
{
	Heal = 0,
	Ammo = 1,
	DamageMult = 2,
	
};

UCLASS()
class UNREALPROJ_API AACollectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACollectable();

	UStaticMeshComponent* StaticMesh;

	UAudioComponent* AudioComponent;

	
	UPROPERTY(EditAnywhere)
	USoundBase* CollectSFX;

	ECollectableType CollectableType = ECollectableType::Heal;


	float HealGiveAmmount = 50.0;

	int AmmoGiveAmmount = 20;

	float DamageMultiplier = 2.0;
	float DamageMultiplierDuration = 10.0;

	UPROPERTY(EditAnywhere)
	UMaterial* Redmat;
	UPROPERTY(EditAnywhere)
	UMaterial* Greenmat;
	UPROPERTY(EditAnywhere)
	UMaterial* BlueMat;

	void SetVisible(bool bVisible);

	
	bool bSpawned = false;

	AACollectableSpawner* Spawner = nullptr;
	
	// typedef void(*funcPointer)(AActor*);
	//
	// funcPointer CollectFuntionCall = nullptr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetUp();
	
private:

	UFUNCTION()
	void CollidedWith(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void Heal(AMainCharacter* Player);
	void Ammo(AMainCharacter* Player);
	void DamageMult(AMainCharacter* Player);

	bool bCollected = false;

};
