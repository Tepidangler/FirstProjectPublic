// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "EnemyBase.h"
#include "AIController.h"
#include "Components/BillboardComponent.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	RootComponent = SpawningBox;
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return Point;
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if (SpawnArray.Num() > 0)
	{
		int32 Selection = FMath::FRandRange(0, SpawnArray.Num() - 1);

		return SpawnArray[Selection];
	}
	else
	{
		return nullptr;
	}
}

void ASpawnVolume::SpawnActor_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		FActorSpawnParameters SpawnParams;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f), SpawnParams);

		AEnemyBase* Enemy = Cast<AEnemyBase>(SpawnedActor);
		
		if (Enemy)
		{
			Enemy->SpawnDefaultController();

			AAIController* AICont = Cast<AAIController>(Enemy->GetController());
			
			if (AICont)
			{
				Enemy->AIController = AICont;
			}
		}


	}
}