// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASwitchBase::ASwitchBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	/**Setting up Collisions specfically to Pawns to Overlap SEE: COLLISION DROPDOWN IN BLUEPRINTS*/
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);

	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 32.f));

	Switch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	Switch->SetupAttachment(GetRootComponent());

	SwitchObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchObject"));
	SwitchObject->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ASwitchBase::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASwitchBase::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ASwitchBase::OnOverlapEnd);
	
}

// Called every frame
void ASwitchBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitchBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin."));
}


void ASwitchBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	UE_LOG(LogTemp, Warning, TEXT("Overlap End."));
}