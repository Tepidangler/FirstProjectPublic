// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "CharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectileBase.h"
#include "DrawDebugHelpers.h"

/** Probably TimeSlicing or something idk yet
if (y == 0.f)
{
	int x, y;

	x = 5.f;

	y = fmod(DeltaTime, x);
	UE_LOG(LogTemp, Warning, TEXT("Desperation = %f"), Desperation)
}
*/

//Defining static member variable
bool AEnemyBase::bIsUltimateAttack = false;

AAIController* AEnemyBase::StaticAIController;

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	LeftCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftCombatCollison"));
	LeftCombatCollision->SetupAttachment(GetMesh(), FName("EnemySocketL"));

	RightCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RightCombatCollison"));
	RightCombatCollision->SetupAttachment(GetMesh(), FName("EnemySocketR"));

	bOverlappingAgroSphere = false;
	bOverlappingCombatSphere = false;
	bLeftCollisionOverlapped = false;
	bRightCollisionOverlapped = false;
	bAttacking = false;
	bHasValidTarget = false;
	bMeleeCombat = true;
	bShooting = false;
	bIsBossEnemy = false;
	bIsChargingIdle = false;
	//Gotta get rid of these and use GAS
	bIsTwinBlast = false;
	bIsZinx = false;
	bIsNarbash = false;
	bIsRevenant = false;
	bIsGideon = false;
	bIsShinibi = false;

	


	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;

	Health = 75.f;
	MaxHealth = 100.f;
	Damage = 10.f;

	AttackMinTime = .5f;
	AttackMaxTime = 3.5f;

	DeathDelay = 3.f;

	InterpSpeed = 5.f;
	Desperation = 0.f;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());

	AEnemyBase::StaticAIController = Cast<AAIController>(GetController());


	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::AgroSphereOnOverlapEnd);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AgroSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::CombatSphereOnOverlapEnd);
	CombatSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	LeftCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::LeftCombatCollisionOnOverlapBegin);
	LeftCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::LeftCombatCollisionOnOverlapEnd);
	RightCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::RightCombatCollisionOnOverlapBegin);
	RightCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemyBase::RightCombatCollisionOnOverlapEnd);

	LeftCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LeftCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LeftCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	RightCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBossEnemy && bOverlappingAgroSphere)
	{
		Raycast(CastHit);
	}

	if (bCharging && IsCharacterInView() && bIsChargingIdle && bIsTwinBlast)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection(FName("ChargeBlastFire"), CombatMontage);
			bIsChargingIdle = false;
		}

	}

	if (bUltimateAttack && IsCharacterInView())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection(FName("UltimateFire"), CombatMontage);
			bUltimateAttack = false;
		}
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::SetEnemyMovementStatus(EEnemyMovementStatus Status)
{
	EnemyMovementStatus = Status;
	
}

/** Collision Implementations */

void AEnemyBase::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlappingAgroSphere = true;
	if (OtherActor && Alive())
	{
		ACharacterBase* Target = Cast<ACharacterBase>(OtherActor);
		if (Target)
		{
			MoveToTarget(Target);
		}
	}
}

void AEnemyBase::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ACharacterBase* Target = Cast<ACharacterBase>(OtherActor);
		if (Target)
		{
			bHasValidTarget = false;			
			Target->SetHasCombatTarget(false);
			Target->UpdateCombatTarget();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController)
			{
				AIController->StopMovement();
			}
		}
	}
}

void AEnemyBase::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		ACharacterBase* MC = Cast<ACharacterBase>(OtherActor);
		if (MC)
		{
			bHasValidTarget = true;
			MC->SetCombatTarget(this);
			MC->SetHasCombatTarget(true);
			MC->UpdateCombatTarget();
			CombatTarget = MC;
			bOverlappingCombatSphere = true;
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyBase::Attack, AttackTime, false);
		}	
	}
}

void AEnemyBase::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherComp)
	{
		ACharacterBase* MC = IFirstInterface::Execute_SetPlayerRef(OtherActor);
		if (MC)
		{
			if (MC->CombatTarget == this)
			{
				MC->SetCombatTarget(nullptr);
				MC->bHasCombatTarget = false;
				MC->UpdateCombatTarget();
				bOverlappingCombatSphere = false;
				MoveToTarget(MC);
				CombatTarget = nullptr;	
			}
			if (MC->MainPlayerController)
			{
				USkeletalMeshComponent* MCMesh = Cast<USkeletalMeshComponent>(OtherComp);
				if (MCMesh) MC->MainPlayerController->RemoveEnemyHealthBar();
			}
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
	}
}

void AEnemyBase::LeftCombatCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (bAttacking)
		{
			ACharacterBase* MC = IFirstInterface::Execute_SetPlayerRef(OtherActor);
			if (MC)
			{
				if (MC->HitParticles)
				{
						const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName("TipSocketL");
					if (WeaponSocket)
					{
						FVector SocketLocation = WeaponSocket->GetSocketLocation(GetMesh());
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MC->HitParticles, SocketLocation, FRotator(0.f), false);
					}

				}
				if (MC->HitSound)
				{
					UGameplayStatics::PlaySound2D(this, MC->HitSound);
				}

				if (DamageTypeClass)
				{
					UGameplayStatics::ApplyDamage(MC, Damage, AIController, this, DamageTypeClass);
				}
			}
		}
		
	}
}

void AEnemyBase::LeftCombatCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bLeftCollisionOverlapped = false;
}

void AEnemyBase::RightCombatCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (bAttacking)
		{
			ACharacterBase* MC = IFirstInterface::Execute_SetPlayerRef(OtherActor);
			if (MC)
			{
				if (MC->HitParticles)
				{
					const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName("TipSocketR");
					if (WeaponSocket)
					{
						FVector SocketLocation = WeaponSocket->GetSocketLocation(GetMesh());
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MC->HitParticles, SocketLocation, FRotator(0.f), false);
					}

				}
				if (MC->HitSound)
				{
					UGameplayStatics::PlaySound2D(this, MC->HitSound);
				}
				if (DamageTypeClass)
				{
					UGameplayStatics::ApplyDamage(MC, Damage, AIController, this, DamageTypeClass);
				}
			}
		}
		
	}
}

void AEnemyBase::RightCombatCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bRightCollisionOverlapped = false;
}

void AEnemyBase::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
	Location = GetMesh()->GetSocketLocation(FName("head"));
	Rotation = GetActorRotation();
	Rotation.Yaw -= GetMesh()->GetSocketTransform(FName("head", ERelativeTransformSpace::RTS_ParentBoneSpace)).Rotator().Roll;
}

bool AEnemyBase::Raycast(FHitResult &HitResult)
{
	

	float CastLength = 500.f;

	FVector Location;
	FRotator Rotation;
	GetActorEyesViewPoint(Location, Rotation);

	FVector StartLocation = Location;

	FVector EndLocation = StartLocation + (GetActorForwardVector() * CastLength);

	FCollisionQueryParams CollisionParams;
	
	GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_Visibility, CollisionParams))
	{
		return true;
	}
	return false;
	
}

void AEnemyBase::ActivateCollision()
{
	if (bLeftCollisionOverlapped)
	{
		LeftCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	if (bRightCollisionOverlapped)
	{
		RightCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}

void AEnemyBase::DeactivateCollision()
{
	if (!bLeftCollisionOverlapped)
	{
		LeftCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (!bRightCollisionOverlapped)
	{
		RightCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}





/**Combat Implementations */

void AEnemyBase::Attack()
{
	int32 Section;


	if (bIsBossEnemy)
	{
		Damage = FMath::RandRange(10.f, 100.f);
		DistanceToTarget = FMath::PointDistToLine(CombatTarget->GetActorLocation(), GetActorForwardVector(), GetActorLocation());
	}

	if (Alive() && bHasValidTarget)
	{
		if (AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if (!bAttacking && bMeleeCombat)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			check(AnimInstance);
			if (bIsShinibi || bIsNarbash)
			{
				SetInterpToPlayer(true);
				FRotator TipYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("TipSocket_R"));
				FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), TipYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

				SetActorRotation(InterpRotation);

				Section = FMath::RandRange(0, 2);
				switch (Section)
				{
				case 0:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("AttackA"), CombatMontage);
					break;

				case 1:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("AttackB"), CombatMontage);
					break;

				case 2:
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("AttackC"), CombatMontage);
					break;
				}
			}
			else
			{
				AnimInstance->Montage_Play(CombatMontage, 1.35f);
				AnimInstance->Montage_JumpToSection(FName("AttackA"), CombatMontage);
			}


			

		}

		/// <summary>
		/// If you're reading this, then I want you to know this is definitely not the way to do this and there are infinitely better ways to accomplish what this is, in fact once I actually ship this I'm going to completely change this
		/// but for now this will work since I just need to see that this works.
		/// </summary>
		if (!bShooting && !bMeleeCombat && bIsTwinBlast || bIsRevenant || bIsGideon)
		{
			bShooting = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				if (Desperation >= 1.f && bIsTwinBlast)
				{
					Section = 3;
				}
				else
				{
					Section = FMath::RandRange(0, 2);
				}
				switch (Section)
				{
				case 0:
					CalculateDesperation();
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("Attack1"), CombatMontage);
					SetInterpToPlayer(true);
					check(bInterpToPlayer);
					if (CombatTarget && (bRightGunFiring || bLeftGunFiring) && DistanceToTarget > 34.f)
					{
						if (bIsTwinBlast || bIsRevenant)
						{
							FRotator MuzzleYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("Muzzle_L"));
							FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), MuzzleYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

							SetActorRotation(InterpRotation);
						}
						else
						{
							FRotator MuzzleYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("Muzzle_R"));
							FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), MuzzleYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

							SetActorRotation(InterpRotation);
						}


					}
					Fire();
					break;

				case 1:
					CalculateDesperation();
					AnimInstance->Montage_Play(CombatMontage, 1.35f);
					AnimInstance->Montage_JumpToSection(FName("Attack2"), CombatMontage);
					SetInterpToPlayer(true);
					check(bInterpToPlayer);
					if (CombatTarget && (bRightGunFiring || bLeftGunFiring) && DistanceToTarget > 34.f && !bUltimateAttack && !bCharging)
					{
						if (bIsTwinBlast)
						{
							FRotator MuzzleYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("Muzzle_R"));
							FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), MuzzleYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

							SetActorRotation(InterpRotation);
						}
						else
						{
							FRotator MuzzleYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("Muzzle_L"));
							FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), MuzzleYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

							SetActorRotation(InterpRotation);
						}
						

					}
					Fire();
					break;

				case 2:
					if (!bCharging)
					{
						CalculateDesperation();
						AnimInstance->Montage_Play(CombatMontage, 1.35f);
						if (bIsTwinBlast)
						{
							AnimInstance->Montage_JumpToSection(FName("ChargeBlast"), CombatMontage);
						}
						else
						{
							AnimInstance->Montage_JumpToSection(FName("Attack3"), CombatMontage);
						}
						SetInterpToPlayer(true);
						check(bInterpToPlayer);
						if (CombatTarget && (bLeftGunFiring || bRightGunFiring) && DistanceToTarget > 34.f)
						{
							if (bIsTwinBlast || bIsRevenant)
							{
								FRotator MuzzleYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("Muzzle_L"));
								FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), MuzzleYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

								SetActorRotation(InterpRotation);
							}
							else
							{
								FRotator MuzzleYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("Muzzle_R"));
								FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), MuzzleYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

								SetActorRotation(InterpRotation);
							}
						}
						bCharging = true;
					}
					break;

				case 3:
					if (Desperation >= 1.f)
					{
						CalculateDesperation();
						AnimInstance->Montage_Play(CombatMontage, 1.35f);
						AnimInstance->Montage_JumpToSection(FName("UltimateStart"), CombatMontage);
						SetInterpToPlayer(true);
						check(bInterpToPlayer);
						if (CombatTarget && bLeftGunFiring && DistanceToTarget > 34.f)
						{
							FRotator MuzzleYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("Muzzle_L"));
							FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), MuzzleYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

							SetActorRotation(InterpRotation);
						}
						bUltimateAttack = true;
						AEnemyBase::bIsUltimateAttack = true;
					}
					break;
				default:

					break;

				}
			}
			if (EffortSound && !bCharging)
			{
				UGameplayStatics::PlaySound2D(this, EffortSound);
			}
		}

		if (!bShooting && !bMeleeCombat && bIsZinx)
		{
			bShooting = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			check(AnimInstance);
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("Attack1"), CombatMontage);
			SetInterpToPlayer(true);
			check(bInterpToPlayer);
			if (CombatTarget && bRightGunFiring && DistanceToTarget > 34.f)
			{
				FRotator MuzzleYaw = GetMuzzleRotationYaw(CombatTarget->GetActorLocation(), FName("Muzzle_R"));
				FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), MuzzleYaw, GetWorld()->GetDeltaSeconds(), InterpSpeed);

				SetActorRotation(InterpRotation);
			}
			Fire();
		}
		if (EffortSound && !bCharging)
		{
			UGameplayStatics::PlaySound2D(this, EffortSound);
		}


	}
}

void AEnemyBase::Fire()
{
	if (ProjectileClass)
	{
		FVector LeftMuzzleSocketLocation;
		FVector RightMuzzleSocketLocation;
		FRotator LeftMuzzleSocketRotation;
		FRotator RightMuzzleSocketRotation;

		GetMesh()->GetSocketWorldLocationAndRotation("Muzzle_L", LeftMuzzleSocketLocation, LeftMuzzleSocketRotation);
		GetMesh()->GetSocketWorldLocationAndRotation("Muzzle_R", RightMuzzleSocketLocation, RightMuzzleSocketRotation);

		if (bCharging)
		{
			LeftMuzzleSocketRotation.Pitch -= 15.f;
			RightMuzzleSocketRotation.Pitch -= 15.f;
		}
		else
		{
			LeftMuzzleSocketRotation.Pitch -= 5.f;
			RightMuzzleSocketRotation.Pitch -= 5.f;
		}


		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			if (bLeftGunFiring)
			{
				Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, LeftMuzzleSocketLocation, LeftMuzzleSocketRotation, SpawnParams);
				if (Projectile)
				{
					FVector LaunchDirection = LeftMuzzleSocketRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
					UGameplayStatics::SpawnEmitterAttached(Projectile->ProjectileParticles, Projectile->StaticMeshComponent, FName("ProjectileAttachSocket"), Projectile->StaticMeshComponent->GetRelativeLocation(), FRotator(0.f), FVector(1.f), EAttachLocation::SnapToTarget, true);
					if (!bCharging)
					{
						UGameplayStatics::SpawnEmitterAtLocation(World, WeaponFireParticles, LeftMuzzleSocketLocation, FRotator(0.f), false);
					}

				}
					bCharging = false;
					bLeftGunFiring = false;
					SetInterpToPlayer(false);
			}
			if (bRightGunFiring)
			{

				Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, RightMuzzleSocketLocation, RightMuzzleSocketRotation, SpawnParams);
				if (Projectile)
				{
					FVector LaunchDirection = RightMuzzleSocketRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
					UGameplayStatics::SpawnEmitterAttached(Projectile->ProjectileParticles, Projectile->StaticMeshComponent, FName("ProjectileAttachSocket"), Projectile->StaticMeshComponent->GetRelativeLocation(), FRotator(0.f), FVector(1.f), EAttachLocation::SnapToTarget, true);
					if (!bCharging)
					{
						UGameplayStatics::SpawnEmitterAtLocation(World, WeaponFireParticles, RightMuzzleSocketLocation, FRotator(0.f), false);
					}


				}
					bCharging = false;
					bRightGunFiring = false;
					SetInterpToPlayer(false);
			}

		}
	}

}

void AEnemyBase::AttackEnd()
{
	bAttacking = false;
	bShooting = false;
	bCharging = false;

	if (bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyBase::Attack, AttackTime, false);
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(1.f, CombatMontage);
		}

	}

}

void AEnemyBase::UltimateFire()
{
	if (bIsBossEnemy)
	{
		Damage = FMath::RandRange(10.f, 50.f);
	}
	
	if (ProjectileClass)
	{
		FVector LeftMuzzleSocketLocation;
		FVector RightMuzzleSocketLocation;
		FRotator LeftMuzzleSocketRotation;
		FRotator RightMuzzleSocketRotation;

		LeftMuzzleSocketRotation.Pitch -= 15.f;
		RightMuzzleSocketRotation.Pitch -= 15.f;

		GetMesh()->GetSocketWorldLocationAndRotation("Muzzle_L", LeftMuzzleSocketLocation, LeftMuzzleSocketRotation);
		GetMesh()->GetSocketWorldLocationAndRotation("Muzzle_R", RightMuzzleSocketLocation, RightMuzzleSocketRotation);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

					Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, (LeftMuzzleSocketLocation), LeftMuzzleSocketRotation, SpawnParams);
					if (Projectile)
					{
						FVector LaunchDirection = LeftMuzzleSocketRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						UGameplayStatics::SpawnEmitterAttached(Projectile->ProjectileUltimateParticles, Projectile->StaticMeshComponent, FName("ProjectileAttachSocket"), Projectile->StaticMeshComponent->GetRelativeLocation(), FRotator(0.f), FVector(.5f), EAttachLocation::SnapToTarget, false);
						if (!bCharging)
						{
							UGameplayStatics::SpawnEmitterAtLocation(World, WeaponUltimateFireParticles, LeftMuzzleSocketLocation, FRotator(0.f), false);
						}

					}

					Projectile = World->SpawnActor<AProjectileBase>(ProjectileClass, (RightMuzzleSocketLocation), RightMuzzleSocketRotation, SpawnParams);
					if (Projectile)
					{
						FVector LaunchDirection = RightMuzzleSocketRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);
						UGameplayStatics::SpawnEmitterAttached(Projectile->ProjectileUltimateParticles, Projectile->StaticMeshComponent, FName("ProjectileAttachSocket"), Projectile->StaticMeshComponent->GetRelativeLocation(), FRotator(0.f), FVector(.5f), EAttachLocation::SnapToTarget, false);
						if (!bCharging)
						{
							UGameplayStatics::SpawnEmitterAtLocation(World, WeaponUltimateFireParticles, RightMuzzleSocketLocation, FRotator(0.f), false);
						}


					}
				SetInterpToPlayer(false);
		}
	}
	bIsUltimateAttack = false;
}

void AEnemyBase::UltimateAttackEnd()
{
	bUltimateAttack = false;
	bRightGunFiring = false;
	bLeftGunFiring = false;
	bAttacking = false;
	bShooting = false;

	if (bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyBase::Attack, AttackTime, false);
	}
	else
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Stop(1.f, CombatMontage);
		}

	}
}

void AEnemyBase::LeftCombatAboutToCollide()
{
	bLeftCollisionOverlapped = true;
}

void AEnemyBase::RightCombatAboutToCollide()
{
	bRightCollisionOverlapped = true;
}

void AEnemyBase::LeftGunAboutToFire()
{
	bLeftGunFiring = true;
}

void AEnemyBase::RightGunAboutToFire()
{
	bRightGunFiring = true;
}

void AEnemyBase::SpawnMuzzleFlashParticles()
{
	if (bLeftGunFiring)
	{
		if (WeaponFireParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponFireParticles, GetMesh()->GetSocketLocation(FName("Muzzle_L")), FRotator(0.f), false);
		}
		
	}

	if (bRightGunFiring)
	{
		if (WeaponFireParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponFireParticles, GetMesh()->GetSocketLocation(FName("Muzzle_R")), FRotator(0.f), false);
		}
	}
}


/**Sounds Implementations */
void AEnemyBase::PlaySwingSound()
{
	if (SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, SwingSound);
	}
}

void AEnemyBase::PlayEffortSound()
{
	if (EffortSound)
	{
		UGameplayStatics::PlaySound2D(this, EffortSound);
	}
}

void AEnemyBase::PlayDeathSound()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySound2D(this, DeathSound);
	}
}



float AEnemyBase::CalculateDesperation()
{
	if (Health <= 200.f)
	{
		if (Desperation >= 1.f)
		{
			return Desperation = 0.f;
		}
		if (Desperation >= .5f)
		{
			Desperation += FMath::FRandRange(.1f, .5f);
		}
		if (Desperation <= .5f)
		{
			Desperation += FMath::FRandRange(.01f, .1f);
		}
	}

	return Desperation;


}

bool AEnemyBase::IsCharacterInView()
{
	ACharacterBase* MC = Cast<ACharacterBase>(CastHit.GetActor());
	if (MC)
	{
		return true;
	}

	return false;
}

void AEnemyBase::SetbIsChargingIdle()
{
	bIsChargingIdle = true;
}

void AEnemyBase::MoveToTarget(ACharacterBase* Target)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

	if (AIController)
	{

		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(10.f);

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);

		//auto PathPoints = NavPath->GetPathPoints();
		/**
				for (auto Points : PathPoints)
				{
					FVector Location = Points.Location;

					UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);		}
				}
		*/
	}
}

FRotator AEnemyBase::GetMuzzleRotationYaw(FVector Target, FName SocketName)
{
	FRotator MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(SocketName), Target);
	
	FRotator MuzzleRotationYaw(0.f, MuzzleRotation.Yaw, 0.f);

	return MuzzleRotationYaw;
}

void AEnemyBase::SetInterpToPlayer(bool Interp)
{

	bInterpToPlayer = Interp;
}

AEnemyBase* AEnemyBase::SetEnemyRef_Implementation()
{
	return this;
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		if (Alive())
		{
			Die(DamageCauser);
		}
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}

void AEnemyBase::Die(AActor* Causer)
{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage && Alive())
		{
			AnimInstance->Montage_Play(CombatMontage, 1.f);
			AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
			check(DeathSound);
			PlayDeathSound();
			//AnimInstance->Montage_Stop(1.f, CombatMontage);
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
			GetWorld()->GetTimerManager().PauseTimer(AttackTimer);
		}

		LeftCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RightCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore); // Commented out because I don't want players running through dead enemy actors

		ACharacterBase* MC = Cast<ACharacterBase>(Causer);
		if (MC)
		{
			MC->UpdateCombatTarget();
		}

}

void AEnemyBase::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	if (DeathParticles)
	{
		FVector Location = GetMesh()->GetComponentLocation();
		FVector ParticleSpawnLocation(Location.X,Location.Y,(Location.Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticles, Location, FRotator(0.f), true);
	}
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemyBase::Disappear, DeathDelay);
}

void AEnemyBase::Disappear()
{
	Destroy();
}

bool AEnemyBase::Alive()
{
	return GetEnemyMovementStatus()  !=  EEnemyMovementStatus::EMS_Dead;
}

