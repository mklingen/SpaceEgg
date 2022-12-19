// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageCallback.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageCallback : public UInterface
{
	GENERATED_BODY()
};

/**
 * Callback interface which receives damage. Needs a Health component to work.
 */
class SPACEEGG_API IDamageCallback
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage")
	void TakeDamage(float Damage, const UDamageType* DamageType);

};
