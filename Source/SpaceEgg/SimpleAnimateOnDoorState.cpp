// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAnimateOnDoorState.h"

#include "Door.h"
#include "Helpers/PrintHelper.h"

// Sets default values for this component's properties
USimpleAnimateOnDoorState::USimpleAnimateOnDoorState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void USimpleAnimateOnDoorState::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void USimpleAnimateOnDoorState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void USimpleAnimateOnDoorState::OnDoorStateChanged_Implementation(const TSoftObjectPtr<ADoor>& door)
{
	if (!door.IsValid())
	{
		LOGE("Door invalid somehow.");
		return;
	}
	auto doorPtr = door.Get();
	auto doorState = doorPtr->GetState();
	// If closed or closing and our state is open, then close.
	if (doorState == DoorState::ClosedAndLocked || 
		doorState == DoorState::ClosedAndUnlocked ||
		doorState == DoorState::Closing)
	{
		if (IsOpenOrOpening())
		{
			Animate(GetClosingDirection(), doorPtr->AnimationTime);
		}
	}
	// If open or opening and our state is closed, then open.
	else if (doorState == DoorState::Open || doorState == DoorState::Opening)
	{
		if (IsClosedOrClosing())
		{
			Animate(!GetClosingDirection(), doorPtr->AnimationTime);
		}
	}
}

bool USimpleAnimateOnDoorState::IsClosedOrClosing() const 
{
	if (ClosedIsA)
	{
		return (state == SimpleAnimationState::StoppedAtA || state == SimpleAnimationState::MovingFromBToA);
	}
	else
	{
		return (state == SimpleAnimationState::StoppedAtB || state == SimpleAnimationState::MovingFromAToB);
	}
}

bool USimpleAnimateOnDoorState::IsOpenOrOpening() const
{
	return !IsClosedOrClosing();
}
bool USimpleAnimateOnDoorState::GetClosingDirection() const 
{
	return !ClosedIsA;
}