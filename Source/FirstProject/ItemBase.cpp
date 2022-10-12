// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Volume"));
	RootComponent = CollisionVolume;
	
	IdleParticlesComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Idle Explosion Particles"));
	IdleParticlesComponent->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	//Rotation of the Mesh
	bRotate = false;
	RotationRate = 45.f;

	//Hovering of the Mesh
	bHover = false;
	A = 0.f;
	B = 0.f;
	C = 0.f;
	D = 0.f;
	
	
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnOverlapEnd);
	BaseZLocation = GetActorLocation().Z;
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);

	}

	if (bHover)
	{
		FVector NewLocation = GetActorLocation();
		// BaseZLocation plus Amplitude times sin function of B times RunningTime - Phase Shift and then add Vertical Shift
		NewLocation.Z = BaseZLocation + A * FMath::Sin(B * RunningTime - C) + D; // Period = 2 * Pi / Abs(B)
		SetActorLocation(NewLocation);

		RunningTime += DeltaTime;
	}
}

void AItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Item Super::OnOverlapEnd"));
}

