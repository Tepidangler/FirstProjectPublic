// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMainCharacter::AMainCharacter()
{
	//Set Size for Collision Capsule

	GetCapsuleComponent()->InitCapsuleSize(40.f, 88.f);

	GetCharacterMovement()->JumpZVelocity = 450.f;
	GetCharacterMovement()->AirControl = .125f;

	MaxHealth = 100.f;
	Health = 100.f;
	MaxStamina = 100.f;
	Stamina = 100.f;
	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	Dirhams = 0;

}