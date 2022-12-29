// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PuzzleCompletedCallback.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPuzzleCompletedCallback : public UInterface
{
	GENERATED_BODY()
};

class SPACEEGG_API IPuzzleCompletedCallback
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
		void OnPuzzleCompleted();
};
