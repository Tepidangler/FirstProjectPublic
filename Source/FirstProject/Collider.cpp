// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ColliderMovementComponent.h"

// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	//Creating Sphere Component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->InitSphereRadius(45.f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	//Creating Mesh Component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(SphereComponent);
	// Assigning StaticMesh to Component
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	if (MeshComponentAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		MeshComponent->SetWorldScale3D(FVector(.8f, .8f, .8f));
	}

	//Creating SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(MeshComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;

	//Creating Camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm,USpringArmComponent::SocketName);

	//Creating Movement Component

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("Our Movement Component"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	CameraInput = FVector2D(0.f, 0.f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	NewRotation.Pitch += CameraInput.Y;
	SetActorRotation(NewRotation);

	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.f,-15.f);
	NewSpringArmRotation.Roll = 0.f;

	SpringArm->SetWorldRotation(NewSpringArmRotation);


}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::CameraPitch);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::CameraYaw);


}


//TODO: Fix movement in Positive Z direction
void ACollider::MoveForward(float Value)
{
	FVector Forward = GetActorForwardVector();

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(Forward  * Value);

		//UE_LOG(LogTemp, Warning, TEXT("Z = %f"),Forward.Z)
	}

}


void ACollider::MoveRight(float Value)
{
	FVector Right = GetActorRightVector();

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(Right * Value);
	}


}

void ACollider::CameraPitch(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void ACollider::CameraYaw(float AxisValue)
{
	CameraInput.X = AxisValue;

}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return OurMovementComponent;

}