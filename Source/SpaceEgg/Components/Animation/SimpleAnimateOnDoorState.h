// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceEgg/Components/Animation/SimpleAnimation.h"
#include "SpaceEgg/Actors/Doors/DoorStateListener.h"

#include "SimpleAnimateOnDoorState.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API USimpleAnimateOnDoorState : public USimpleAnimation, public IDoorStateListener
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleAnimateOnDoorState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Door State")
		bool ClosedIsA = true;
	
	virtual void OnDoorStateChanged_Implementation(const TSoftObjectPtr<ADoor>& door) override;

	bool IsClosedOrClosing() const;
	bool IsOpenOrOpening() const;
	bool GetClosingDirection() const;
};
