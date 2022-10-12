// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "EnemyBase.h"
#include "FirstInterface.h"

AExplosive::AExplosive()
{
	Damage = 15.f;
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex,bFromSweep,SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("Explosive::OverlapBegin"));

	if (OtherActor)
	{
		//ACharacterBase* MC = Cast<ACharacterBase>(OtherActor);
		ACharacterBase* MC = IFirstInterface::Execute_SetPlayerRef(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		AEnemyBase* Enemy = IFirstInterface::Execute_SetEnemyRef(OtherActor);
		FString OAName = OtherActor->GetName();
		if (MC || Enemy)
		{
			UE_LOG(LogTemp, Warning, TEXT("Other Actor: %s"), *OAName)
			UWorld* World = GetWorld();
			if (OverlapParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			UGameplayStatics::ApplyDamage(OtherActor, Damage, nullptr, this, DamageTypeClass);
			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex);

	UE_LOG(LogTemp, Warning, TEXT("Explosive::OverlapEnd"));
	
}