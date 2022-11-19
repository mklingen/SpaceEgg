// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionTrigger.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionTrigger : public UInterface
{
	GENERATED_BODY()
};

/**
 * Handles generic interaction (on/off) from the user, for toggles and such.
 */
class SPACEEGG_API IInteractionTrigger
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void OnInteraction();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		void OnCursorOver();
};
