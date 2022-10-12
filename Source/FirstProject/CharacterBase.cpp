// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyBase.h"
#include "MainPlayerController.h"
#include "FirstSaveGame.h"
#include "ItemStorage.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	/** Create Camera Boom. Pulls Towards player if there's a collision */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f; //Camera Follows at this distance
	CameraBoom->bUsePawnControlRotation = true; // Rotate Arm based on controller

	/** Create Follow Camera. */
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust 
	// to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; 

	//Set Our Turn rate for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	// Don't rotate when the controller rotates
	// Let that just affect the camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Configure Character Movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // ... at this rotation rate
	

	// Set Default Yaw Rotation for Mesh to face Forward
	ACharacter::GetMesh()->SetRelativeRotation(FRotator(0.f,-90.f,0.f));


	//Setting Running and Sprinting Default
	RunningSpeed = 500.f;
	SprintingSpeed = 600.f;
	//Setting Interpolation Speed
	InterpSpeed = 15.f;

	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;

	bMovingForward = false;
	bMovingRight = false;
	bShiftKeyDown = false;
	bActionDown = false;
	bIsWeaponEquipped = false;
	bIsInAir = false;
	bInterpToEnemy = false;
	bHasCombatTarget = false;
	bIsComboStarted = false;
	bESCDown = false;

	//Initialize Enums
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;


	
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation() + FVector(0.f,0.f,175.f), 25.f, 8, FLinearColor::Green, 10.f, .5f);
	
	MainPlayerController = Cast<AMainPlayerController>(GetController());
	bAllowMovement = false;

	LoadGameNoSwitch();
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//bIsInAir = this->GetCharacterMovement()->IsFalling();
	//Caluclating how much will drain this frame with delta time
	if (Alive() == false) return;
	float DeltaStamina = StaminaDrainRate * DeltaTime;
	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			if (bMovingForward || bMovingRight)
			{
				if (Stamina - DeltaStamina <= MinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Fatigued);
					Stamina -= DeltaStamina;
				}
				else
				{
					Stamina -= DeltaStamina;
				}
				if (bMovingForward || bMovingRight)
				{
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
				else
				{
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
			}
			else // Shift Key Up 
			{
				if (Stamina + DeltaStamina >= MaxStamina)
				{
					Stamina = MaxStamina;
				}
				else
				{
					Stamina += DeltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			
		}
		else // Shift Key Up 
		{
			if (Stamina + DeltaStamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Fatigued:
		if (bShiftKeyDown)
		{
			if (bMovingForward || bMovingRight)
			{
				if (Stamina - DeltaStamina <= 0.f)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
					Stamina = 0.f;
					SetMovementStatus(EMovementStatus::EMS_Normal);
				}
				else // Otherwise just decrement stamina
				{
					Stamina -= DeltaStamina;
					if (bMovingForward || bMovingRight)
					{
						SetMovementStatus(EMovementStatus::EMS_Sprinting);
					}
					else
					{
						SetMovementStatus(EMovementStatus::EMS_Normal);
					}
				}
			}
			else //NOT Moving Right/Left OR Forward/Backward
			{
				if (Stamina + DeltaStamina >= MinSprintStamina)
				{
					SetStaminaStatus(EStaminaStatus::ESS_Normal);
					Stamina += DeltaStamina;
				}
				else
				{
					Stamina += DeltaStamina;
				}
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			
		}
		else //Shift Key Up
		{
			if (Stamina + DeltaStamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
				Stamina += DeltaStamina;
			}
			else
			{
				Stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			if (bMovingForward || bMovingRight)
			{
				Stamina = 0.f;
			}
			else //Moving Right/Left OR Forward/Backward
			{
				SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
				Stamina += DeltaStamina;
			}
		}
		else //Shift Key Up
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;

	default:
		;
	}

	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}

	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();

		if (MainPlayerController)
		{
			MainPlayerController->EnemyLocation = CombatTargetLocation;
		}
	}
}

FRotator ACharacterBase::GetLookAtRotationYaw(FVector Target)
{

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);

	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);

	return LookAtRotationYaw;
}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	//Bind Movement Functionality to Keyboard
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpRate);
	//Bind Jump Action to Keyboard
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBase::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Bind Turn Functionality to Mouse
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//Bind Shift Action to Keyboard
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACharacterBase::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACharacterBase::ShiftKeyUp);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &ACharacterBase::ActionDown);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &ACharacterBase::ActionUp);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ACharacterBase::LMBDown);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ACharacterBase::LMBUp);

	PlayerInputComponent->BindAction("OpenPauseMenu", IE_Pressed, this, &ACharacterBase::ESCDown);
	PlayerInputComponent->BindAction("OpenPauseMenu", IE_Released, this, &ACharacterBase::ESCUp);
	
}


void ACharacterBase::MoveForward(float Value)
{
	bMovingForward = false;
	if (Controller != nullptr && Value != 0.f && !bAttacking && Alive() && bAllowMovement)
	{
		//find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//Get Rotation Matrix from YawRotation's X axis from a Unit Vector (X)
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		bUseControllerRotationYaw = true; // While Character is moving allow Camera to use Yaw Rotation
		AddMovementInput(Direction, Value);
		bMovingForward = true;

	}
	else
	{
		bUseControllerRotationYaw = false; // While Character is not moving return to default
	}

}


void ACharacterBase::MoveRight(float Value)
{
	bMovingRight = false;
	if (Controller != nullptr && Value != 0.f && !bAttacking && Alive() && bAllowMovement)
	{
		//find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//Get Rotation Matrix from YawRotation's X axis from a Unit Vector (X)
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		
		AddMovementInput(Direction, Value);
		bMovingRight = true;

	}

}


void ACharacterBase::TurnAtRate(float Rate)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		AddControllerYawInput(Rate * BaseTurnRate * World->GetDeltaSeconds());
	}
}

void ACharacterBase::LookUpRate(float Rate)
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * World->GetDeltaSeconds());
	}
}

void ACharacterBase::DecrementHealth(float Amount)
{
	if (Health - Amount <= 0.f)
	{
		Die();
	}
	else
	{
		Health -= Amount;
	}
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		Die();
		if (DamageCauser)
		{
			AEnemyBase* Enemy = Cast<AEnemyBase>(DamageCauser);
			if (Enemy)
			{
				Enemy->bHasValidTarget = false;
			}
		}
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void ACharacterBase::Die()
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage && Alive())
	{
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
		//AnimInstance->Montage_Stop(1.f, CombatMontage);
		//GetWorld()->GetTimerManager().PauseTimer(ComboTimerHandle);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
}

void ACharacterBase::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}


void ACharacterBase::AddDirhams(int32 Amount)
{
	if (Dirhams + Amount <= 99999)
	{
		Dirhams += Amount;
	}
	else
	{
		int32 Difference = 99999 - Dirhams;
		Dirhams += Difference;
	}
}


void ACharacterBase::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void ACharacterBase::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void ACharacterBase::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void ACharacterBase::ActionDown()
{

	bActionDown = true;
	if (ActiveOverlappingItem)
	{
		AWeaponBase* Weapon = Cast<AWeaponBase>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
}

void ACharacterBase::ActionUp()
{

	bActionDown = false;
}


void ACharacterBase::LMBDown()
{
	if (!Alive()) return;

	if (!bAttacking)
	{
		//If Overlapping Item is Nullptr
		if (ActiveOverlappingItem == nullptr)
		{
			//... And Equipped Weapon is Valid
			if (EquippedWeapon)
			{
				//Attack
				Attack();
			}
		}
	}


}


void ACharacterBase::LMBUp()
{
		bAttacking = false;
}


void ACharacterBase::Attack()
{
	//GetWorld()->GetTimerManager().PauseTimer(ComboTimerHandle);
	if (!bAttacking && Alive())
	{
		bAttacking = true;
		SetInterpToEnemy(true);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			if (bIsComboStarted == false)
			{
				int32 Section = FMath::RandRange(0, 3);
				switch (Section)
				{
				case 0:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("PrimaryAttack_1"), CombatMontage);
					bIsComboStarted = true;
					//GetWorldTimerManager().SetTimer(ComboTimerHandle, 1.5f, false);
					break;

				case 1:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("PrimaryAttack_B"), CombatMontage);
					bIsComboStarted = true;
					//GetWorldTimerManager().SetTimer(ComboTimerHandle, 1.5f, false);
					break;

				case 2:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("PrimaryAttack_C"), CombatMontage);
					bIsComboStarted = true;
					//GetWorldTimerManager().SetTimer(ComboTimerHandle, 1.5f, false);
					break;
				case 3:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("PrimaryAttack_D"), CombatMontage);
					bIsComboStarted = true;
					//GetWorldTimerManager().SetTimer(ComboTimerHandle, 1.5f, false);
					break;

				default:

					break;

				}
				/**
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("PrimaryAttack_1"), CombatMontage);
				GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, 1.5f, true, 1.5f);
				*/
				if (AnimInstance && CombatMontage && this->GetCharacterMovement()->IsFalling())
				{
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("PrimaryAttack_Air"), CombatMontage);
					AnimInstance->Montage_Stop(1.f, CombatMontage);
				}
				if (SwingSwordEffortSound)
				{
					UGameplayStatics::PlaySound2D(this, SwingSwordEffortSound);
				}
			}

		}
	
		/**
		if (EquippedWeapon->SwingSound)
		{
			UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
		}
		
		if (!bAttacking)
		{
			AttackEnd(FName("PrimaryAttack_Recover"));
		}
		*/

	}
	
}

void ACharacterBase::ESCDown()
{
	bESCDown = true;
	if (MainPlayerController)
	{
		MainPlayerController->TogglePauseMenu();
	}
}

void ACharacterBase::ESCUp()
{
	bESCDown = false;
}

void ACharacterBase::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;

}

bool ACharacterBase::Alive()
{
	return GetMovementStatus() != EMovementStatus::EMS_Dead;
}

void ACharacterBase::Jump()
{
	if (MovementStatus != EMovementStatus::EMS_Dead)
	{
		ACharacter::Jump();
	}
}



void ACharacterBase::ShowPickupLocations()
{
	/**
	//Using a for loop to draw a debug sphere for picked up Dirhams
	for (int32 i = 0; i < PickupLocations.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, PickupLocations[i], 25.f, 8, FLinearColor::Green, 10.f, .5f);
	}
	*/

	for (auto Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Green, 10.f, .5f);
	}
	
}

void ACharacterBase::PlaySwingSound()
{
	if (EquippedWeapon)
	{
		if (EquippedWeapon->SwingSound)
		{
			UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
		}
	}
	
}

void ACharacterBase::UpdateCombatTarget()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors,EnemyFilter);

	if (OverlappingActors.Num() == 0)
	{
		if (MainPlayerController)
		{
			MainPlayerController->RemoveEnemyHealthBar();
		}
		return;
	}

	AEnemyBase* ClosestEnemy = Cast<AEnemyBase>(OverlappingActors[0]);
	if (ClosestEnemy)
	{
		FVector CharLocation = GetActorLocation();
		float MinDistance = (ClosestEnemy->GetActorLocation() - GetActorLocation()).Size();
		float Distance;

		for (auto Actor : OverlappingActors)
		{
			AEnemyBase* Enemy = Cast<AEnemyBase>(Actor);
			if (Enemy)
			{
				Distance = (Enemy->GetActorLocation() - GetActorLocation()).Size();
				if (Distance < MinDistance)
				{
					MinDistance = Distance;
					ClosestEnemy = Enemy;
				}
			}
		}
		if (MainPlayerController)
		{
			MainPlayerController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(ClosestEnemy);
		bHasCombatTarget = true;
	}
	



}

void ACharacterBase::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (World)
	{
		FString CurrentLevel = World->GetMapName();

		FName CurrentLevelName(*CurrentLevel);
		if (CurrentLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(World, LevelName);
		}
	}

}

void ACharacterBase::SaveGame()
{
	UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health = Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Stamina = Stamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Dirhams = Dirhams;
	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();
	FString MapName = GetWorld()->GetMapName();
	MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	SaveGameInstance->CharacterStats.MapName = MapName;


	if (EquippedWeapon)
	{
		SaveGameInstance->CharacterStats.WeaponName = EquippedWeapon->WeaponName;
	}

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SlotName, SaveGameInstance->UserIndex);

	SavedGameArray.Add(SaveGameInstance);
	
}

void ACharacterBase::LoadGame(bool bSetPosition)
{
	UFirstSaveGame* LoadGame = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGame->SlotName, LoadGame->UserIndex));

	if (LoadGameInstance)
	{
		Health = LoadGameInstance->CharacterStats.Health;
		MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
		Stamina = LoadGameInstance->CharacterStats.Stamina;
		MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
		Dirhams = LoadGameInstance->CharacterStats.Dirhams;

		if (WeaponStorage)
		{
			AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
			if (Weapons)
			{
				FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
				if (Weapons->WeaponMap.Contains(WeaponName))
				{
					AWeaponBase* WeaponToEquip = GetWorld()->SpawnActor<AWeaponBase>(Weapons->WeaponMap[WeaponName]);
					WeaponToEquip->Equip(this);
				}

			}
		}

		if (!Alive())
		{
			SetMovementStatus(EMovementStatus::EMS_Normal);
			GetMesh()->bPauseAnims = false;
			GetMesh()->bNoSkeletonUpdate = false;
		}
		

		if (bSetPosition)
		{
			SetActorLocation(LoadGameInstance->CharacterStats.Location);
			SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
		}

		if (LoadGameInstance->CharacterStats.MapName != TEXT(""))
		{
			FName MapName(* LoadGameInstance->CharacterStats.MapName);
			SwitchLevel(MapName);
		}

		check(MainPlayerController);
		FInputModeGameOnly GameOnly;
		MainPlayerController->SetInputMode(GameOnly);
	}

	


}

void ACharacterBase::LoadGameNoSwitch()
{
	UFirstSaveGame* LoadGame = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGame->SlotName, LoadGame->UserIndex));

	if (LoadGameInstance)
	{
		Health = LoadGameInstance->CharacterStats.Health;
		MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
		Stamina = LoadGameInstance->CharacterStats.Stamina;
		MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
		Dirhams = LoadGameInstance->CharacterStats.Dirhams;
		if (WeaponStorage)
		{
			AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
			if (Weapons)
			{
				FString WeaponName = LoadGameInstance->CharacterStats.WeaponName;
				if (Weapons->WeaponMap.Contains(WeaponName))
				{
					AWeaponBase* WeaponToEquip = GetWorld()->SpawnActor<AWeaponBase>(Weapons->WeaponMap[WeaponName]);
					WeaponToEquip->Equip(this);
				}

			}
		}

		if (!Alive())
		{
			SetMovementStatus(EMovementStatus::EMS_Normal);
			GetMesh()->bPauseAnims = false;
			GetMesh()->bNoSkeletonUpdate = false;
		}
	}
}

void ACharacterBase::SetEquippedWeapon(AWeaponBase* WeaponToSet)
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
		bIsWeaponEquipped = false;
	}

	EquippedWeapon = WeaponToSet;
	bIsWeaponEquipped = true;
}

void ACharacterBase::AttackEnd(FName AttackRecoverySection)
{
	bAttacking = false;
	bIsComboStarted = false;
	SetInterpToEnemy(false);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.35f);
		AnimInstance->Montage_JumpToSection(AttackRecoverySection, CombatMontage);
		//GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
		//GetWorldTimerManager().PauseTimer(ComboTimerHandle);

	}

}


//Use this instead of casting
ACharacterBase* ACharacterBase::SetPlayerRef_Implementation() { return this; }

