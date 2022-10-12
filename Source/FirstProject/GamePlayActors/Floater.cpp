// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"


// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));
	
	InitialLocation = FVector(0.0f);
	PlacedLocation = FVector(0.0f);
	WorldOrigin = FVector(0.0f, 0.0f, 0.0f);
	bInitializedFloaterLocations = false;
	bShouldFloat = false;
	InitialDirection = FVector(0.0f,0.0f, 0.0f);
	InitialForce = FVector(200000.f, 0.0f, 0.0f);
	InitialTorque = FVector(200000.f, .0f, .0f);
	
	RunningTime = 0.f;
	A = .0f;
	B = .0f;
	C = .0f;
	D = .0f;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	PlacedLocation = GetActorLocation();


	float InitialX = FMath::FRandRange(-500.f,500.f);
	float InitialY = FMath::FRandRange(-500.f, 500.f);
	float InitialZ = FMath::FRandRange(0.f,500.f);

	InitialLocation.X = InitialX;
	InitialLocation.Y = InitialY;
	InitialLocation.Z = InitialZ;

	if (bInitializedFloaterLocations)
	{
		SetActorLocation(InitialLocation);
	}
	
	BaseZLocation = PlacedLocation.Z;

	StaticMesh->AddTorqueInRadians(InitialTorque);
}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldFloat)
	{

		FVector NewLocation = GetActorLocation();
		// BaseZLocation plus Amplitude times sin function of B times RunningTime - Phase Shift and then add Vertical Shift
		NewLocation.Z = BaseZLocation + A * FMath::Sin(B *RunningTime - C) + D; // Period = 2 * Pi / Abs(B)
		SetActorLocation(NewLocation);

		RunningTime += DeltaTime;

	}

}

