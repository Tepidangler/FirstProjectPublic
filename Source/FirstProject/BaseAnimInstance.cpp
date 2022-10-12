// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


void UBaseAnimInstance::NativeInitializeAnimation()
{
	//Check to see if Pawn is nullpointer
	if (Pawn == nullptr)
	{
		//Sets Pawn to PawnOwner if it can
		Pawn = TryGetPawnOwner();


	}

	

}

void UBaseAnimInstance::NativeBeginPlay()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		MC = Cast<ACharacterBase>(Pawn);
		if (MC)
		{
			if (MC->LevelStartMontage)
			{
				Montage_Play(MC->LevelStartMontage);
			}
		}
	}
}

void UBaseAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			MC = Cast<ACharacterBase>(Pawn);
		}
	}

	if (Pawn)
	{
		UWorld* World = GetWorld();
		//Set Speed
		FVector Velocity = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Velocity.X, Velocity.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		//Set bIsInAir
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		//Set Roll, Pitch, and Yaw
		FRotator AimRotation = Pawn->GetBaseAimRotation();
		FRotator ActorRotation = Pawn->GetActorRotation();
		Roll = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Roll;
		Pitch = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Pitch;
		Yaw = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, ActorRotation).Yaw;

		//Set Yaw Delta
		RotationLastTick = Pawn->GetActorRotation();
		TargetRotation = (UKismetMathLibrary::NormalizedDeltaRotator(RotationLastTick, ActorRotation).Yaw / World->GetDeltaSeconds()) / 10;
		YawDelta = FMath::FInterpTo(YawDelta, TargetRotation, World->GetDeltaSeconds(), 6.f);

		//Set bisFullBody
		float CurveValue = GetCurveValue("FullBody");
		if (CurveValue > 0)
		{
			bIsFullBody = true;
		}


		if (Pawn)
		{
			MC = Cast<ACharacterBase>(Pawn);

			//Set Acceleration boolean
			if (MC)
			{
				float CurrentAcceleration = MC->GetCharacterMovement()->GetCurrentAcceleration().Size();
				if (CurrentAcceleration > 0)
				{
					bIsAccelerating = true;
				}
				else
				{
					bIsAccelerating = false;
				}

				bIsWeaponEquipped = MC->bIsWeaponEquipped;
			}
			
		}
		if (Pawn)
		{
			MC = Cast<ACharacterBase>(Pawn);
			if (MC)
			{
				if (MC->LevelStartMontage)
				{
					if (!Montage_IsPlaying(MC->LevelStartMontage))
					{
						MC->SetAllowMovementStatus(true);
					}
				}
			}
		}
	}
}

FName UBaseAnimInstance::GetNextAttackRecoverySection()
{
	FName AttackRecoverySectionName;

	if (Pawn)
	{ 
		//MC = Cast<ACharacterBase>(Pawn);
		FName CurrentMontageSection = Montage_GetCurrentSection();

		if (CurrentMontageSection == "PrimaryAttack_1")
		{
			AttackRecoverySectionName = AttackRecoverySections[0];
		}
		if (CurrentMontageSection == "PrimaryAttack_B")
		{
			AttackRecoverySectionName = AttackRecoverySections[1];
		}
		if (CurrentMontageSection == "PrimaryAttack_C")
		{
			AttackRecoverySectionName = AttackRecoverySections[2];
		}
		if (CurrentMontageSection == "PrimaryAttack_D")
		{
			AttackRecoverySectionName = "None";
		}
		if (CurrentMontageSection == "PrimaryAttack_Air")
		{
			AttackRecoverySectionName = "None";
		}
	}
	return AttackRecoverySectionName;
}

void UBaseAnimInstance::PlaySwingEffortSound()
{
	if (Pawn)
	{
		MC = Cast<ACharacterBase>(Pawn);
		if (MC)
		{
			if (MC->SwingSwordEffortSound)
			{
				UGameplayStatics::PlaySound2D(this, MC->SwingSwordEffortSound);
			}
		}

	}

}
