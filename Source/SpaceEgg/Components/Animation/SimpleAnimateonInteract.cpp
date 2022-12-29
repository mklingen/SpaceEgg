// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAnimateonInteract.h"

#include "SpaceEgg/Helpers/ActorHelpers.h"
#include "SpaceEgg/Interfaces/Toggleable.h"

// Sets default values for this component's properties
USimpleAnimateonInteract::USimpleAnimateonInteract()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USimpleAnimateonInteract::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USimpleAnimateonInteract::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USimpleAnimateonInteract::OnInteraction_Implementation()
{
	if (state == SimpleAnimationState::StoppedAtA || state == SimpleAnimationState::MovingFromBToA)
	{
		Animate(true, AnimationTime);
	}
	else if (state == SimpleAnimationState::StoppedAtB || state == SimpleAnimationState::MovingFromAToB)
	{
		Animate(false, AnimationTime);
	}
}
