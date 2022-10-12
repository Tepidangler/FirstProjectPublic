// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PotionPickup.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API APotionPickup : public AItemBase
{
	GENERATED_BODY()
	

public:
	APotionPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;


public:
	//Functions
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	void AddHealth(AActor* Player);
};
