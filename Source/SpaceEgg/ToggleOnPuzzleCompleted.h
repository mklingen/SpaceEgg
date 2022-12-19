// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleCompletedCallback.h"
#include "ToggleOnPuzzleCompleted.generated.h"

// Calls all toggleable components whenever the puzzle is completed.
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UToggleOnPuzzleCompleted : public UActorComponent, public IPuzzleCompletedCallback
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UToggleOnPuzzleCompleted();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Puzzle")
	int StateOnPuzzleIncomplete = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Puzzle")
	int StateOnPuzzleComplete = 1;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void ToggleAll(bool complete);

public:	

	virtual void OnPuzzleCompleted_Implementation() override;
		
};
