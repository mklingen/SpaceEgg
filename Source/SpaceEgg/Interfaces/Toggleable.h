// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Toggleable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UToggleable : public UInterface
{
	GENERATED_BODY()
};


class SPACEEGG_API IToggleable
{
	GENERATED_BODY()

public:
	// Reset the puzzle to its starting condition.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SpaceEgg/Interfaces/Toggleable")
		int GetState() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "SpaceEgg/Interfaces/Toggleable")
		void SetState(int state);
};
