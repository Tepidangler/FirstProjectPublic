// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FirstInterface.h"
#include "CharacterBase.h"
#include "EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystem.h"
#include "AIController.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.f);
	RootComponent = CollisionComponent;
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));



	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->Bounciness = .05f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 3.f;

	Damage = 10.f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileBase::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	

	if (OtherActor != this && OtherActor)
	{
		//OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.f, Hit.ImpactPoint);
		//ACharacterBase* MC = IFirstInterface::Execute_SetPlayerRef(OtherActor);

		FString Name = OtherActor->GetName();
		UE_LOG(LogTemp,Warning,TEXT("Other Actor Name: %s"), *Name)

		ACharacterBase* MC = Cast<ACharacterBase>(OtherActor); // offender
		if (MC)
		{
			UGameplayStatics::ApplyDamage(MC, Damage, AEnemyBase::StaticAIController, this, DamageTypeClass);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitCharParticles, Hit.ImpactPoint, FRotator(0.f), true);
		}
		else
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitWorldParticles, Hit.ImpactPoint, FRotator(0.f), true);

		}
		if (AEnemyBase::bIsUltimateAttack && MC)
		{
			UGameplayStatics::ApplyDamage(MC, Damage, AEnemyBase::StaticAIController, this, DamageTypeClass);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltHitCharParticles, Hit.ImpactPoint, FRotator(0.f), true);
		}
		else if (AEnemyBase::bIsUltimateAttack && !MC)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UltHitWorldParticles, Hit.ImpactPoint, FRotator(0.f), true);
		}

		Destroy();
	}


}

