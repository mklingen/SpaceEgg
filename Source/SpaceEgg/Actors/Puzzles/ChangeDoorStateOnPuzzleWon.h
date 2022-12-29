// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleCompletedCallback.h"
#include "ChangeDoorStateOnPuzzleWon.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UChangeDoorStateOnPuzzleWon : public UActorComponent, public IPuzzleCompletedCallback
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChangeDoorStateOnPuzzleWon();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
		class ADoor* door = nullptr;

public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door State")
		bool OpenDoor = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Door State")
		bool LockDoor = false;
	virtual void OnPuzzleCompleted_Implementation() override;

};
