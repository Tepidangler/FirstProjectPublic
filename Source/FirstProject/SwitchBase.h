// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwitchBase.generated.h"

UCLASS()
class FIRSTPROJECT_API ASwitchBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASwitchBase();

	/** Overlap volume for functionality to be triggered*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Switch")
		class UBoxComponent* TriggerBox;

	/** Switch to Character to Step-On*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Switch")
		class UStaticMeshComponent* Switch;

	/** Object that does something when Switch is triggered*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Switch")
		UStaticMeshComponent* SwitchObject;

	FTimerHandle SwitchHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	float SwitchTime;

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