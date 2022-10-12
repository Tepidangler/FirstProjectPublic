// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AEnemyBase>(Pawn);
		}
	}

}

void UEnemyAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			Enemy = Cast<AEnemyBase>(Pawn);

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
			Enemy = Cast<AEnemyBase>(Pawn);
			//Set Acceleration boolean
			if (Enemy)
			{
				FRotator CurrentRotation = Enemy->GetActorRotation();
				float CurrentAcceleration = Enemy->GetCharacterMovement()->GetCurrentAcceleration().Size();
				if (CurrentAcceleration > 0)
				{
					bIsAccelerating = true;
				}
				else
				{
					bIsAccelerating = false;
				}

				Direction = (UKismetMathLibrary::NormalizedDeltaRotator(CurrentRotation, Enemy->GetVelocity().Rotation())).Yaw * -1.f;
			}

		}

		if (bIsAccelerating)
		{
			StopDirection = Direction;
		}
	}


}


