// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SwitchBase.h"
#include "FloorSwitch.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTPROJECT_API AFloorSwitch : public ASwitchBase
{
	GENERATED_BODY()
public:
	AFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerFloorSwitch();
	
	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateSwitchObjectLocation(float Z);

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateSwitchLocation(float Z);

	void CloseDoor();

	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


	bool bCharacterOnSwitch;


	//Initial Location for Object
	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialSwitchObjectLocation;

	//Initial Location for Floor Switch
	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialSwitchLocation;

	
};
