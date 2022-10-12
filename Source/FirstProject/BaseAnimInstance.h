// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float YawDelta;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	FRotator RotationLastTick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class ACharacterBase* MC;
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsWeaponEquipped;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Montage | Section Names")
	TArray<FName> AttackRecoverySections{ FName("PrimaryAttackA_Recover"), FName("PrimaryAttackB_Recover"), FName("PrimaryAttackC_Recover") };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Montage | Section Names")
	TArray<FName> MontageSections{ FName("PrimaryAttack_1"), FName("PrimaryAttack_B"), FName("PrimaryAttack_C"), FName("PrimaryAttack_D"), FName("PrimaryAttack_Air") };

	//Functions
	virtual void NativeInitializeAnimation() override;

	virtual void NativeBeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable, Category = "Montage | Section Name")
	FName GetNextAttackRecoverySection();

	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void PlaySwingEffortSound();





	
};
