// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	//Resets Input vector back to 0 and then normalizes it 
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.f) * DeltaTime * MaxSpeed;

	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		//Will be filled by SafeMoveUpdatedComponent if there is a hit
		FHitResult Hit;
		//Takes DesiredMovement and the Movement Components rotation and we pass in a HitResult
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);
		
		// If we bump into something slide along the side of it
		if (Hit.IsValidBlockingHit())
		{
			//Takes DesiredMovement, Time since we hit surface, vector of orientation the direction of 
			//the surface we hit, the hit itself and computes where to move the pawn if we were to move along the surface 
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		//	UE_LOG(LogTemp, Warning, (TEXT("Valid Blocking HIt!")))
		}
	}
}