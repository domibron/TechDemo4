// Fill out your copyright notice in the Description page of Project Settings.



#include "MainCharacter.h"




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
	bPLayerHasUI = false;
	PlayerID = GetPlatformUserId();

	GLog->Log("ID is " + GetPlatformUserId());
	
	CameraComponent = FindComponentByClass<UCameraComponent>();
	NiagaraSystem = FindComponentByClass<UNiagaraComponent>();

	
	
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
		GLog->Log( GetName() +" Found UI " + FString::FromInt(FoundWidgets.Num()) + " " + FString::FromInt(GetPlatformUserId()));
		bPLayerHasUI = true;
	}
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	
	if(bPLayerHasUI) // I hate player spawning. would override but cannot be bothered. this works fine :3
	{
		
		
		// if (PlayerID == 0)
		// {
			PlayerUI->PlayerOneHealthPercentage = healthRes;
			PlayerUI->PlayerOneHealthText = FString::SanitizeFloat(floor(CurrentHealth));
			PlayerUI->PlayerOneWeaponString = (bReloading) ? "Reloading [" + FString::SanitizeFloat((floor(ReloadTimeLeft * 100)) / 100) + "]" : FString::FromInt(AmmoInClip) + " / " + FString::FromInt(AmmoPool);

			PlayerUI->DamageColor = FLinearColor(1.0, 0.0,0.0, LerpFloat(0, PlayerUI->DamageMaxTransparancyAmmount, DamageIndicatorTimeRemaining / DamageIndicatorDuration));

			PlayerUI->DamageMultColor = FLinearColor(1.0, 1.0,1.0, LerpFloat(0, 1, DamageMultiplierDurationRemainder));
		

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

		FVector StartLocation = CameraComponent->GetComponentLocation();

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
		NiagaraSystem->SetVectorParameter("StartLocation", GetActorLocation()+FVector(0, 0, 60));
	}
	else if (bLaserActive)
	{
		NiagaraSystem->UnregisterComponent();
		NiagaraSystem->Deactivate();
		bLaserActive = false;
	}


	
	
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
	if (bReloading) return;

	int ammoToTake = 0;
	
	if (AmmoPool >= MaxClipSize)
	{
		ammoToTake = MaxClipSize - AmmoInClip;
	}
	else if (AmmoPool < MaxClipSize && AmmoPool > 0)
	{
		ammoToTake = AmmoPool;
	}

	AmmoInClip += ammoToTake;
	AmmoPool -= ammoToTake;
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
	if (currentFireTimeLeft > 0 || AmmoInClip <= 0 || bReloading) return;

	currentFireTimeLeft = 1.0 / fireRate;
	AmmoInClip -= 1;
	
	FHitResult Hit;

	FVector StartLocation = CameraComponent->GetComponentLocation();

	FVector EndLocation = (CameraComponent->GetForwardVector() * 200000)+StartLocation;

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
		GLog->Log("Hit: " + Hit.GetActor()->GetName());
		
		AMainCharacter* player = Cast<AMainCharacter>(Hit.GetActor());

		if (player)
		{
			if(Hit.GetComponent()->ComponentHasTag("Head"))
			{
				player->TakeDamage(headDamage * DamageMultiplier);
			}
			else if(Hit.GetComponent()->ComponentHasTag("Body"))
			{
				player->TakeDamage(bodyDamage * DamageMultiplier);
				
			}
			
			
		}
	}
}

void AMainCharacter::Aim()
{
	CameraComponent->FieldOfView = 30.0f;
	bAiming = true;
}

void AMainCharacter::UnAim()
{
	CameraComponent->FieldOfView = 90.0f;
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
	BeginReload();
}

void AMainCharacter::JumpPressed()
{
	Jump();
}