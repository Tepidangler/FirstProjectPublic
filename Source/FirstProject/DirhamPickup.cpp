// Fill out your copyright notice in the Description page of Project Settings.


#include "DirhamPickup.h"
#include "CharacterBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

ADirhamPickup::ADirhamPickup()
{
	
}

void ADirhamPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	DirhamCount = FMath::FRandRange(1.f, 1000.f);

	if (OtherActor)
	{
		ACharacterBase* MC = Cast<ACharacterBase>(OtherActor);
		if (MC)
		{
			UWorld* World = GetWorld();
			if (OverlapParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			MC->AddDirhams(DirhamCount);
			MC->PickupLocations.Add(GetActorLocation());
			Destroy();
		}

	}
}

void ADirhamPickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

}