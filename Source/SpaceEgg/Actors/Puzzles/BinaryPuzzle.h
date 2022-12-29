// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleInterface.h"
#include "BinaryPuzzle.generated.h"

UENUM()
enum BinaryPuzzleOperator
{
	Equal     UMETA(DisplayName = "Equal"),
	NotEqual  UMETA(DisplayName = "Not Equal"),
	Or   UMETA(DisplayName = "Or"),
	And UMETA(DisplayName = "And"),
	Neither UMETA(DisplayName = "Neither")
};

UCLASS()
class SPACEEGG_API UBinaryPuzzle : public UActorComponent, public IPuzzleInterface
{
	GENERATED_BODY()
public:

	UBinaryPuzzle();
	virtual void BeginPlay() override;

	virtual bool IsPuzzleWon_Implementation() override;
	virtual void WinPuzzle_Implementation() override;
	virtual void LosePuzzle_Implementation() override;
	virtual void ResetPuzzle_Implementation() override;

	UFUNCTION(BlueprintCallable)
		bool Set(int col, int idx, bool value);

	UFUNCTION(BlueprintCallable)
		bool CheckSolution(int idx) const;

	UFUNCTION(BlueprintCallable)
		TEnumAsByte <BinaryPuzzleOperator> GetOperator() const { return op; }

	UFUNCTION(BlueprintCallable)
		int GetSize() const { return Size;  }

	UFUNCTION(BlueprintCallable)
		bool GetState(int column, int idx) const;

	UFUNCTION(BlueprintCallable)
		bool GetState1(int idx) const;

	UFUNCTION(BlueprintCallable)
		bool GetState2(int idx) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binary Puzzle")
		int Size = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binary Puzzle")
		TEnumAsByte<BinaryPuzzleOperator> op;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binary Puzzle")
		TArray<bool> state1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Binary Puzzle")
		TArray<bool> state2;

	bool IsIndexValid(int idx) const { return idx >= 0 && idx < Size; }
};

