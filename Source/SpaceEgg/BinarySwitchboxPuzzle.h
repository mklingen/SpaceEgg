// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveObject.h"
#include "BinaryPuzzle.h"
#include "BinarySwitchboxPuzzle.generated.h"

class ADoor;
class AInteractiveObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPuzzleComplete);


// Represents a column of elements within the puzzle.
USTRUCT(BlueprintType)
struct FColumn {
	GENERATED_BODY()
public:
	// Origin of the first button e.w.r.t this actor.
	// will have the same orientation as the actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Origin = FVector(0, 0, 0);
	// Direction of the column e.w.r.t the actor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Direction = FVector(0, 1, 0);
	// Distance between each element in the column.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Spacing = 0.1;
};

UCLASS()
class SPACEEGG_API ABinarySwitchboxPuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	ABinarySwitchboxPuzzle();

protected:
	virtual void BeginPlay() override;
	// Called when the object is moved or if parameters have changed.
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	// The room this puzzle is associated with. Will call callbacks on the room.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Room")
		TWeakObjectPtr<AActor> RoomActor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AInteractiveObject> ControllerButtonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AActor> SuccessClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<ADoor> DoorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Blueprints")
		TSubclassOf<AInteractiveObject> DoorHandleClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWRite, Category = "Door")
		FVector DoorRelativeSpawnLocation = FVector::Zero();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWRite, Category = "Door")
		FRotator DoorRelativeSpawnRotation = FRotator(0);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWRite, Category = "Door")
		FVector DoorHandleRelativeSpawnLocation = FVector::Zero();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWRite, Category = "Door")
		FRotator DoorHandleRelativeSpawnRotation = FRotator(0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
		UBinaryPuzzle* Puzzle;

	UPROPERTY(VisibleInstanceOnly, Category = "Door")
		ADoor* Door = nullptr;
	UPROPERTY(VisibleInstanceOnly, Category = "Door")
		AInteractiveObject* DoorHandle = nullptr;

	UPROPERTY(VisibleInstanceOnly, Category = "Buttons")
		TArray<AInteractiveObject*> Buttons1;
	UPROPERTY(VisibleInstanceOnly, Category = "Buttons")
		TArray<AInteractiveObject*> Buttons2;
	UPROPERTY(VisibleInstanceOnly, Category = "Buttons")
		TArray<AActor*> SuccessIndicators;


	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
		FOnPuzzleComplete OnPuzzleComplete;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Puzzle")
		bool IsPuzzleComplete = false;

	UPROPERTY(EditDefaultsOnly, Category="Buttons")
		FColumn Column1;
	UPROPERTY(EditDefaultsOnly, Category = "Buttons")
		FColumn Column2;
	UPROPERTY(EditDefaultsONly, Category = "Buttons")
		FColumn SuccessColumn;

	UFUNCTION()
		void CreateButtons();

	UFUNCTION()
		void CreateDoor();
	
	UFUNCTION()
		void DestroyChildren();

	UFUNCTION()
		void RecreateChildren();

	UFUNCTION()
		AActor* CreateColumnElement(const FColumn& col, int idx, TSubclassOf<AActor> actor);

	UFUNCTION()
		void OnButtonPushed(int col, int button);

	float TimeLastChanged = 0;
	bool IsFlashingSolution = false;
	int SolutionIdxToFlash = -1;
	bool PropertyChanged = false;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float TimeToFlashSolution = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool DebugDraw = false;

};
