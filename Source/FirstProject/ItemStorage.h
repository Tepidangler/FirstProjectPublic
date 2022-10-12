// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStorage.generated.h"

UCLASS()
class FIRSTPROJECT_API AItemStorage : public AActor
{
	GENERATED_BODY()
	
public:	

	//Variables
	
	// Sets default values for this actor's properties
	AItemStorage();

	//Map of WeaponBlueprints
	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TMap<FString, TSubclassOf<class AWeaponBase>> WeaponMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
