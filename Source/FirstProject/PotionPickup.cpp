// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionPickup.h"
#include "CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

APotionPickup::APotionPickup()
{
}

void APotionPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (OtherActor)
	{
		
		{
			if (OverlapParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f), true);
			}

			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			AddHealth(OtherActor);
			Destroy();
		}
		
		
	}
}

void APotionPickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void APotionPickup::AddHealth(AActor* Player)
{
	ACharacterBase* MC = Cast<ACharacterBase>(Player);

	if (MC)
	{
		if (MC->Health != MC->MaxHealth)
		{
			if ((MC->Health + Health) >= MC->MaxHealth)
			{
				MC->Health = MC->MaxHealth;
			}
			else
			{
				MC->Health += Health;
			}


		}
		else
		{
			return;
		}
	}
	
}
