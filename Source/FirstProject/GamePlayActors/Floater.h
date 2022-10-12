// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class FIRSTPROJECT_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActorMeshComponents")
	UStaticMeshComponent* StaticMesh;

	// Location of the Actor used by SetActorLocation when BeginPlay is called
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVariables")
	FVector InitialLocation;

	// Location of the Actor when dragged in from editor
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "FloaterVariables")
	FVector PlacedLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "FloaterVariables")
	FVector  WorldOrigin;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
	FVector InitialDirection;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "FloaterVariables")
	FVector InitialForce;

	UPROPERTY(BlueprintReadWrite, EditInstanceONly, Category = "FloaterVariables")
	FVector InitialTorque;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
	bool bShouldFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FloaterVariables")
	bool bInitializedFloaterLocations;

	//Amplitude - How much we oscilate up and down
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
		float A;
	//Period = 2 * Pi / (absolute value of B)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
		float B;
	// Phase Shift is C / B
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
		float C;

	//Vertical Shift is D
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
		float D;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	float RunningTime;

	float BaseZLocation;


};
