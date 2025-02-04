// Fill out your copyright notice in the Description page of Project Settings.



#include "MainCharacter.h"

#include "MyCustomPlayerController.h"


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
	
	MainGameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	
	bPLayerHasUI = false;
	if (GetController()) PlayerID = Cast<AMyCustomPlayerController>(GetController())->GetLocalPlayer()->GetLocalPlayerIndex();
	GLog->Log("ID is " + PlayerID);
	
	CameraComponent = FindComponentByClass<UCameraComponent>();
	NiagaraSystem = FindComponentByClass<UNiagaraComponent>();
	AudioComponent =  FindComponentByClass<UAudioComponent>();
	SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();

	CameraBoom = FindComponentByClass<USpringArmComponent>();

	LaserStartMesh = FindComponentByTag<UStaticMeshComponent>("LaserStart");
	
	CurrentHealth = MaxHealth;

	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UPlayerUIUSerWidget::StaticClass(), true);
	
	for (int i = 0; i < FoundWidgets.Num(); i++)
	{
		if (FoundWidgets[i]->GetOwningPlayer() == GetController())
		{
			PlayerUI = Cast<UPlayerUIUSerWidget>(FoundWidgets[i]);
		}
	}
	
	// if (FoundWidgets.Num() > 0 && PlayerID < FoundWidgets.Num() && PlayerID >= 0)
	// {
	// 	UUserWidget* foundUI = FoundWidgets[PlayerID];
	// 	if (foundUI->GetOwningPlayer() == GetController())
	// 	if (foundUI) PlayerUI = Cast<UPlayerUIUSerWidget>(foundUI);
	// }
	
	
 
	if (PlayerUI)
	{
		GLog->Log( GetName() +" Found UI " + FString::FromInt(FoundWidgets.Num()) + " " + FString::FromInt(PlayerID));
		bPLayerHasUI = true;
	}

	if (SkeletalMesh && PlayerOneMat && PlayerTwoMat && !bSetMat)
	{
		if (PlayerID == 0)
		{
			SkeletalMesh->SetMaterial(0, PlayerOneMat);
			bSetMat = true;
		}
		else if (PlayerID == 1)
		{
			SkeletalMesh->SetMaterial(0, PlayerTwoMat);
			bSetMat = true;
		}
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}

	if (GetController() && PlayerID == -1) PlayerID = Cast<AMyCustomPlayerController>(GetController())->GetLocalPlayer()->GetLocalPlayerIndex();
	
	if (SkeletalMesh && PlayerOneMat && PlayerTwoMat  && !bSetMat)
	{
		if (PlayerID == 0)
		{
			SkeletalMesh->SetMaterial(0, PlayerOneMat);
			bSetMat = true;
		}
		else if (PlayerID == 1)
		{
			SkeletalMesh->SetMaterial(0, PlayerTwoMat);
			bSetMat = true;
		}

		
		
		GLog->Log(FString::FromInt(PlayerID));
		
	}
	
	if (CurrentHealth <= 0)
	{
		AudioComponent->SetSound(DeathSFX);
		AudioComponent->Play();

		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->SetPhysicsBlendWeight(1.0);
		SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		if (AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->PlayerDied(PlayerID);
		}

		
	}
	
	if (DamageMultiplierDurationRemainder > 0)
	{
		DamageMultiplierDurationRemainder -= DeltaTime;
	}
	else
	{
		DamageMultiplier = 1.0f;
	}
	
	if (DamageIndicatorTimeRemaining > 0)
	{
		DamageIndicatorTimeRemaining -= DeltaTime;
	}
	
	if (ReloadTimeLeft > 0)
	{
		bReloading = true;
		ReloadTimeLeft -= DeltaTime;
	}
	else if (bReloading){
		bReloading = false;
		ReloadAmmo();
	}
	
	if (currentFireTimeLeft > 0)
	{
		currentFireTimeLeft -= DeltaTime;
	}

	float healthRes = CurrentHealth / MaxHealth;

	if (CameraBoom)
	{
		FRotator camRotation = CameraBoom->GetComponentRotation();

		camRotation.Pitch = FMath::Clamp(camRotation.Pitch, 50, -50);
	}

	if (HitMarkerTimer > 0) HitMarkerTimer -= DeltaTime;
	
	if(bPLayerHasUI) // I hate player spawning. would override but cannot be bothered. this works fine :3
	{
		
		
		// if (PlayerID == 0)
		// {
			PlayerUI->PlayerOneHealthPercentage = healthRes;
			PlayerUI->PlayerOneHealthText = FString::SanitizeFloat(floor(CurrentHealth));
			PlayerUI->PlayerOneWeaponString = (bReloading) ? "Reloading [" + FString::SanitizeFloat((floor(ReloadTimeLeft * 100)) / 100) + "]" : FString::FromInt(AmmoInClip) + " / " + FString::FromInt(AmmoPool);

			PlayerUI->DamageColor = FLinearColor(1.0, 0.0,0.0, LerpFloat(0, PlayerUI->DamageMaxTransparancyAmmount, DamageIndicatorTimeRemaining / DamageIndicatorDuration));

		PlayerUI->DamageMultColor = FLinearColor(1.0, 1.0,1.0, LerpFloat(0, 1, DamageMultiplierDurationRemainder));

		HitMarkerColor.A = LerpFloat(0, 1, HitMarkerTimer / 0.3);
		
		PlayerUI->HitMarkerColor = HitMarkerColor;
		

		//

		// }
		// else
		// {
		// 	PlayerUI->PlayerTwoHealthPercentage = healthRes;
		// 	PlayerUI->PlayerTwoHealthText = FString::SanitizeFloat(floor(CurrentHealth));
		// 	PlayerUI->PlayerTwoWeaponString = (bReloading) ? "Reloading [" + FString::SanitizeFloat((floor(ReloadTimeLeft * 100)) / 100) + "]" : FString::FromInt(AmmoInClip) + " / " + FString::FromInt(AmmoPool);
		//
		// }
	}
	else
	{
		TArray<UUserWidget*> FoundWidgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), FoundWidgets, UPlayerUIUSerWidget::StaticClass(), true);

		for (int i = 0; i < FoundWidgets.Num(); i++)
		{
			if (FoundWidgets[i]->GetOwningPlayer() == GetController())
			{
				PlayerUI = Cast<UPlayerUIUSerWidget>(FoundWidgets[i]);
			}
		}
		
		// if (FoundWidgets.Num() > 0 && PlayerID < FoundWidgets.Num() && PlayerID >= 0)
		// {
		// 	UUserWidget* foundUI = FoundWidgets[PlayerID];
		// 		
		// 	if (foundUI) PlayerUI = Cast<UPlayerUIUSerWidget>(foundUI);
		// }
		
		
		
		if (PlayerUI)
		{
			GLog->Log("Found UI");
			bPLayerHasUI = true;
		}
	}
	
	//if (AutoReceiveInput == 1) GLog->Log("Player1");
	//if (AutoReceiveInput == 2) GLog->Log("Player2");
	//GLog->Log(FString::FromInt(AutoReceiveInput));

	if (bAiming && ADSLerpTime < 1)
	{
		ADSLerpTime += DeltaTime * 15.0;
	}
	else if (!bAiming && ADSLerpTime > 0)
	{
		ADSLerpTime -= DeltaTime * 15.0;
	}

	CameraComponent->FieldOfView = FMath::Lerp(90.0, 30.0, ADSLerpTime);
	
	if (NiagaraSystem && bAiming)
	{
		if(!bLaserActive)
		{
			NiagaraSystem->Activate();
			NiagaraSystem->RegisterComponent();
			bLaserActive = true;
		}
		
		// we could store this constantly. then use it for damage and laser.
		FHitResult LaserHit;

		// FVector StartLocation = CameraComponent->GetComponentLocation();

		FVector StartLocation  = LaserStartMesh->GetComponentLocation();
		
		FVector EndLocation = (CameraComponent->GetForwardVector() * 200000)+StartLocation;

		FCollisionQueryParams CollisionParams = FCollisionQueryParams();

		CollisionParams.bDebugQuery = false;
		CollisionParams.bTraceComplex = true;
		CollisionParams.TraceTag = TEXT("PLAYER");
		CollisionParams.AddIgnoredActor(this);
		
	
		bool bLaserHit = GetWorld()->LineTraceSingleByChannel(LaserHit, StartLocation, EndLocation, ECollisionChannel::ECC_PhysicsBody, CollisionParams);

		if (bLaserHit)
		{
			NiagaraSystem->SetVectorParameter("EndLocation", LaserHit.ImpactPoint);
		}
		else
		{
			NiagaraSystem->SetVectorParameter("EndLocation", GetActorLocation()+CameraComponent->GetForwardVector()*200000);
		}
		
		NiagaraSystem->SetVectorParameter("StartLocation", StartLocation);
	}
	else if (bLaserActive)
	{
		NiagaraSystem->UnregisterComponent();
		NiagaraSystem->Deactivate();
		bLaserActive = false;
	}


	DotProductCamAngle = DotProductFVector(GetActorUpVector(), CameraComponent->GetForwardVector());
	//GLog->Log(FString::FromInt(DotProductCamAngle) + " " + GetActorUpVector().ToString() + " " + CameraComponent->GetForwardVector().ToString());
	
}

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
		PlayerInputComponent->BindAction("AimKey", IE_Released, this, &AMainCharacter::UnAim);

	PlayerInputComponent->BindAction("ReloadKey", IE_Pressed, this, &AMainCharacter::Reload);

	
		PlayerInputComponent->BindAction("QuitKey", IE_Pressed, this, &AMainCharacter::QuitToMainMenu);


	
		PlayerInputComponent->BindAxis("XStick", this, &AMainCharacter::MoveForward);
		PlayerInputComponent->BindAxis("YStick", this, &AMainCharacter::MoveRight);

		PlayerInputComponent->BindAction("JumpButton", IE_Pressed, this, &AMainCharacter::JumpPressed);

		PlayerInputComponent->BindAxis("XLookStick", this, &AMainCharacter::LookRight);
		PlayerInputComponent->BindAxis("YLookStick", this, &AMainCharacter::LookUp);

		PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &AMainCharacter::Fire);
		PlayerInputComponent->BindAction("AimButton", IE_Pressed, this, &AMainCharacter::Aim);
		PlayerInputComponent->BindAction("AimButton", IE_Released, this, &AMainCharacter::UnAim);

		PlayerInputComponent->BindAction("ReloadButton", IE_Pressed, this, &AMainCharacter::Reload);
	

}

/*
 *test
 */

void AMainCharacter::TakeDamage(float DamageAmount)
{
	CurrentHealth -= DamageAmount;
	GLog->Log(this->GetName() + " " + FString::SanitizeFloat(CurrentHealth));

	
	DamageIndicatorTimeRemaining += DamageIndicatorDuration / 4.0;
	if(DamageIndicatorTimeRemaining > DamageIndicatorDuration) DamageIndicatorTimeRemaining = DamageIndicatorDuration;
}

void AMainCharacter::Heal(float HealAmmount)
{
	CurrentHealth += HealAmmount;
	
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
}

void AMainCharacter::GiveAmmo(int Ammount)
{
	AmmoPool += Ammount;
}

void AMainCharacter::GiveDamageBuff(float Mult, float Duration)
{
	DamageMultiplier = Mult;
	DamageMultiplierDurationRemainder = Duration;
}

float AMainCharacter::LerpFloat(float a, float b, float t)
{
	return a + (b - a) * t;
}

void AMainCharacter::ReloadAmmo()
{

	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}
	
	if (bReloading) return;

	int ammoToTake = 0;
	
	if (AmmoPool >= (MaxClipSize - AmmoInClip))
	{
		ammoToTake = MaxClipSize - AmmoInClip;
	}
	else if (AmmoPool < (MaxClipSize - AmmoInClip) && AmmoPool > 0)
	{
			ammoToTake = AmmoPool;
	}

	AmmoInClip += ammoToTake;
	AmmoPool -= ammoToTake;
}

void AMainCharacter::MoveForward(float Value)
{
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}
	

	//GLog->Log(FString::SanitizeFloat(Value));
	
	AMainCharacter::AddMovementInput(AMainCharacter::GetActorForwardVector(), Value, true);
}

void AMainCharacter::MoveRight(float Value)
{
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}
	

	AMainCharacter::AddMovementInput(AMainCharacter::GetActorRightVector(), Value, true);
}

void AMainCharacter::LookRight(float Value)
{
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}
	
	// FRotator rotation = GetActorRotation();
	//
	// rotation += FRotator(0, Value, 0);
	//
	// SetActorRotation(rotation);

	

	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value)
{
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}

	
	AddControllerPitchInput(Value * -1.0f);
}

// MARK: Fire
void AMainCharacter::Fire()
{
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}
	
	if (currentFireTimeLeft > 0 || AmmoInClip <= 0 || bReloading) return;

	currentFireTimeLeft = 1.0 / fireRate;
	AmmoInClip -= 1;

	AudioComponent->SetSound(ShootSFX);
	AudioComponent->Play();
	
	FHitResult Hit;

	FVector StartLocation = CameraComponent->GetComponentLocation();

	FVector EndLocation = (CameraComponent->GetForwardVector() * 200000) + StartLocation;

	FCollisionQueryParams CollisionParams = FCollisionQueryParams();

	CollisionParams.bDebugQuery = false;
	CollisionParams.bTraceComplex = true;
	CollisionParams.TraceTag = TEXT("PLAYER");
	CollisionParams.AddIgnoredActor(this);

	
	GLog->Log(TEXT("Fire"));

	bool bHasHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECollisionChannel::ECC_PhysicsBody, CollisionParams);

	//bool bDebugHit = false;
	//FHitResult DebugHitRes;

	
	//DrawDebugLineTraceSingle(GetWorld(), StartLocation,EndLocation, EDrawDebugTrace::ForDuration,bDebugHit ,DebugHitRes,FColor::Red,FColor::Green, 5.0f  );
	
	if (bHasHit)
	{
		
		AMainCharacter* player = Cast<AMainCharacter>(Hit.GetActor());

		if (player)
		{
			GLog->Log("Hit: " + Hit.GetActor()->GetName());

			
			if(Hit.GetComponent()->ComponentHasTag("Head"))
			{
				player->TakeDamage(headDamage * DamageMultiplier);

				HitMarkerColor = FLinearColor::Red;
				
				AudioComponent->SetSound(HeadHitSFX);
				AudioComponent->Play();
			}
			else if(Hit.GetComponent()->ComponentHasTag("Body"))
			{
				player->TakeDamage(bodyDamage * DamageMultiplier);

				HitMarkerColor = FLinearColor::White;
				
				AudioComponent->SetSound(BodyHitSFX);
				AudioComponent->Play();
				
			}
			
			HitMarkerTimer = 0.3f;
			
		}
	}
}

void AMainCharacter::Aim()
{
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}
	
	// CameraComponent->FieldOfView = 30.0f;
	bAiming = true;
}

void AMainCharacter::UnAim()
{
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}
	
	//CameraComponent->FieldOfView = 90.0f;
	bAiming = false;
}

void AMainCharacter::BeginReload()
{
	
	if (AmmoPool > 0 && !bReloading && AmmoInClip < MaxClipSize)
	{
		ReloadTimeLeft = ReloadDuration;
	}
	
}

void AMainCharacter::Reload()
{
	if (MainGameMode)
	{
		if (MainGameMode->bGameEnded)
			return;
	}
	
	BeginReload();
}

void AMainCharacter::JumpPressed()
{
	Jump();
}
//MARK: asdsd adas
void AMainCharacter::QuitToMainMenu()
{
	if (AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->LoadLevel("MainMenu");
	}
}

float AMainCharacter::DotProductFVector(FVector v1, FVector v2)
{
	return v1.X * v2.X + v1.Y * v2.Y+ v1.Z * v2.Z;
}
