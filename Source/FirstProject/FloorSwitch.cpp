// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "TimerManager.h"

AFloorSwitch::AFloorSwitch()
{
	SwitchTime = 2.f;
	bCharacterOnSwitch = true;
}

void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

	InitialSwitchObjectLocation = SwitchObject->GetComponentLocation();
	InitialSwitchLocation = Switch->GetComponentLocation();
}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin."));
	if (!bCharacterOnSwitch) bCharacterOnSwitch = true;
	RaiseDoor();
	LowerFloorSwitch();
}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("Overlap End."));
	if (bCharacterOnSwitch) bCharacterOnSwitch = false;
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
}

void AFloorSwitch::UpdateSwitchObjectLocation(float Z)
{
	FVector NewLocation = InitialSwitchObjectLocation;
	NewLocation.Z += Z;
	SwitchObject->SetWorldLocation(NewLocation);
}

void AFloorSwitch::UpdateSwitchLocation(float Z)
{

	FVector NewLocation = InitialSwitchLocation;
	NewLocation.Z += Z;
	Switch->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor()
{
	if (!bCharacterOnSwitch)
	{
		LowerDoor();
		RaiseFloorSwitch();
	}
	

}
