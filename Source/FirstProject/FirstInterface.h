// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FirstInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFirstInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FIRSTPROJECT_API IFirstInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Sets PlayerReference -- USE INSTEAD OF CASTING
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class ACharacterBase* SetPlayerRef();
	//Sets EnemyReference -- USE INSTEAD OF CASTING
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	class AEnemyBase* SetEnemyRef();


};
