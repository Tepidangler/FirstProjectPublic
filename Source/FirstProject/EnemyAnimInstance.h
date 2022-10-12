// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public: //Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AEnemyBase* Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Pitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Roll;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Yaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
		float TargetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bIsAccelerating;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bIsFullBody;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
		bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		float YawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FRotator RotationLastTick;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float StopDirection;


public:
	//Functions
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
		void UpdateAnimationProperties(float DeltaTime);


};
