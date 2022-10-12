// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstInterface.h"
#include "CharacterBase.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	
	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName = "Normal"),
	ESS_Fatigued UMETA(DisplayName = "Fatigued"),
	ESS_Exhausted UMETA(DisplayName = "Exhausted"),
	ESS_ExhaustedRecovering UMETA(DisplayName = "ExhaustedRecovering"),

	ESS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class FIRSTPROJECT_API ACharacterBase : public ACharacter, public IFirstInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Combat")
	FVector CombatTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AWeaponBase* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AItemBase* ActiveOverlappingItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Items | Weapon")
	bool bIsWeaponEquipped;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
	bool bAttacking;

	bool bIsInAir;

	TArray<FVector> PickupLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	/** Camera boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	/** Follow Camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* LevelStartMontage;

	/** Base turn rates to scale turning functions for the camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Running")
	float SprintingSpeed;

	bool bShiftKeyDown;

	bool bActionDown;

	bool bESCDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Movement")
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Movement")
	float MinSprintStamina;

	float InterpSpeed;

	bool bInterpToEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	class AEnemyBase* CombatTarget;

	FTimerHandle ComboTimerHandle;

	bool bIsComboStarted;

	bool bAllowMovement;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Particles")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundCue* SwingSwordEffortSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<AEnemyBase> EnemyFilter;

	UPROPERTY(EditDefaultsOnly, Category = "Save Data")
	TSubclassOf<class AItemStorage> WeaponStorage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save Data")
	TArray<class UFirstSaveGame*> SavedGameArray;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/** Called for forwards/backwards input*/
	void MoveForward(float Value);

	bool bMovingForward;
	/** Called for side to side input*/
	void MoveRight(float Value);

	bool bMovingRight;
	/** Called via input to turn at a given rate
	* @param Rate This is a normalized rate, ie 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/** Called via input to lookup/down at given rate
	* @param Rate This is a normalized rate, ie 1.0 means 100% of desired look rate
	*/
	void LookUpRate(float Rate);

	void DecrementHealth(float Amount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Character Animations")
	void Die();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	void AddDirhams(int32 Amount);

	/** Set Movement status and running speed */
	void SetMovementStatus(EMovementStatus Status);

	FORCEINLINE EMovementStatus GetMovementStatus() { return MovementStatus; }

	//Press Shift Key to Start Sprinting
	void ShiftKeyDown();

	//Release Shift key to stop sprinting
	void ShiftKeyUp();

	//Press Action Button to Interact with objects
	void ActionDown();

	//Release Interact button
	void ActionUp();

	//Press LMB button to Attack
	void LMBDown();

	//Release LMB button 
	void LMBUp();

	//Function for attacking
	void Attack();

	void ESCDown();

	void ESCUp();

	void SetInterpToEnemy(bool Interp);

	bool Alive();

	virtual void Jump() override;

	FRotator GetLookAtRotationYaw(FVector Target);

	UFUNCTION(BlueprintCallable)
	void AttackEnd(FName AttackRecoverySection);

	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	UFUNCTION()
	void UpdateCombatTarget();

	UFUNCTION()
	void SwitchLevel(FName LevelName);

	UFUNCTION(BlueprintCallable)
	void SaveGame();
	UFUNCTION(BlueprintCallable)
	void LoadGame(bool bSetPosition);
	UFUNCTION(BlueprintCallable)
	void LoadGameNoSwitch();

	ACharacterBase* SetPlayerRef_Implementation() override;


	FORCEINLINE void SetAllowMovementStatus(bool Status) { bAllowMovement = Status; }
	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status) { StaminaStatus = Status; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void SetEquippedWeapon(AWeaponBase* WeaponToSet);
	FORCEINLINE AWeaponBase* GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetActiveOverlappingItem(AItemBase* Item) { ActiveOverlappingItem = Item; }

	FORCEINLINE void SetCombatTarget(AEnemyBase* Target) { CombatTarget = Target; }

	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget; }

	/**
	* 
	* 
	* Player Stats
	* 
	* 
	*/

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "PlayerStats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerStats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	int32 Dirhams;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
