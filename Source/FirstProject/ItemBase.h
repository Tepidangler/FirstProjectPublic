// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class FIRSTPROJECT_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

	/**Base Shape Collision*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
	class USphereComponent* CollisionVolume;
	/**Base Static Mesh*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
	class UStaticMeshComponent* Mesh;
	/**Base Particle System Component*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	class UParticleSystemComponent* IdleParticlesComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particle")
	class UParticleSystem* OverlapParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Item | ItemProperties")
	bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	float RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	bool bHover;

	float RunningTime;

	//Base float value of Z Location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
	float BaseZLocation;


/** Handles Hovering of Item using a Sine Wave*/
	//Amplitude - How much we oscilate up and down
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
		float A;
	//Period = 2 * Pi / (absolute value of B)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
		float B;
	// Phase Shift is C / B
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
		float C;

	//Vertical Shift is D
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | ItemProperties")
		float D;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
