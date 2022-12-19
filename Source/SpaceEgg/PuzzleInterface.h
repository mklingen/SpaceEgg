// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PuzzleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPuzzleInterface : public UInterface
{
	GENERATED_BODY()
};


// Generic interface to puzzles. Puzzles must provide a win condition.
class SPACEEGG_API IPuzzleInterface
{
	GENERATED_BODY()

public:
	// Return true when the player has beat the puzzle.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
		bool IsPuzzleWon();
	// Set the puzzle state to the win condition.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
		void WinPuzzle();
	// Set the puzzle state to the lose condition.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
		void LosePuzzle();
	// Reset the puzzle to its starting condition.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
		void ResetPuzzle();
};