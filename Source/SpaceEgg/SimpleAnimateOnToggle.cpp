// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAnimateOnToggle.h"

// Sets default values for this component's properties
USimpleAnimateOnToggle::USimpleAnimateOnToggle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USimpleAnimateOnToggle::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void USimpleAnimateOnToggle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int USimpleAnimateOnToggle::GetState_Implementation() const
{
	return static_cast<int>(toggleValue);
}

void USimpleAnimateOnToggle::SetState_Implementation(int value)
{
	if (value != toggleValue)
	{
		if (state == SimpleAnimationState::StoppedAtA || state == SimpleAnimationState::MovingFromBToA)
		{
			Animate(true, AnimationTime);
		}
		else if (state == SimpleAnimationState::StoppedAtB || state == SimpleAnimationState::MovingFromAToB)
		{
			Animate(false, AnimationTime);
		}
		toggleValue = value;
	}
}