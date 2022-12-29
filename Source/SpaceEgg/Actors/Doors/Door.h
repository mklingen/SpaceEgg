// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class ADoor;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChange, TSoftObjectPtr<ADoor>, door);

UENUM()
enum DoorState
{
	ClosedAndUnlocked     UMETA(DisplayName = "Closed and Unlocked"),
	ClosedAndLocked      UMETA(DisplayName = "Closed and Locked"),
	Open   UMETA(DisplayName = "Open"),
	Opening UMETA(DisplayName = "Opening"),
	Closing UMETA(DisplayName = "Closing")
};

UCLASS()
class SPACEEGG_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		TEnumAsByte<DoorState> state;

	void SetState(TEnumAsByte<DoorState> state);

	void ConnectListeners();

	float timeStartedMoving = 0.0f;
	float timeToStopMoving = 0.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		TEnumAsByte <DoorState> GetState() const;

	UFUNCTION(BlueprintCallable)
		bool CanLock() const;
	UFUNCTION(BlueprintCallable)
		bool CanUnLock() const;
	UFUNCTION(BlueprintCallable)
		bool CanClose() const;
	UFUNCTION(BlueprintCallable)
		bool CanOpen() const;
	UFUNCTION(BlueprintCallable)
		bool LockDoor();
	UFUNCTION(BlueprintCallable)
		bool UnlockDoor();
	UFUNCTION(BlueprintCallable)
		bool OpenDoor();
	UFUNCTION(BlueprintCallable)
		bool CloseDoor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float AnimationTime = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
		bool DebugAutoAnimate = false;

	UPROPERTY(BlueprintAssignable, Category = "State")
		FOnStateChange OnStateChange;

};
