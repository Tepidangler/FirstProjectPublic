// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "DirhamPickup.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API ADirhamPickup : public AItemBase
{
	GENERATED_BODY()
	
public:
	ADirhamPickup();

//Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dirhams")
	int32 DirhamCount;


//Functions

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};