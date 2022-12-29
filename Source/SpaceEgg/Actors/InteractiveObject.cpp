// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractiveObject.h"
#include "SpaceEgg/Helpers/ActorHelpers.h"
#include "SpaceEgg/Interfaces/Toggleable.h"


// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveObject::OnInteraction_Implementation()
{
	activationCount++;
	toggleState = !toggleState;
	OnInteract.Broadcast();
	OnInteractCpp.Broadcast();
	auto interactions = GET_INTERFACES_RECURSIVE(InteractionTrigger, this);
	for (auto& interact : interactions)
	{
		if (interact.GetObject() == this)
		{
			continue;
		}
		interact->OnInteraction_Implementation();
	}

	auto toggles = GET_INTERFACES_RECURSIVE(Toggleable, this);
	for (auto toggle : toggles)
	{
		toggle->SetState_Implementation(toggle->GetState_Implementation() == 0);
	}
}

void AInteractiveObject::OnCursorOver_Implementation()
{
	OnCursor.Broadcast();
	OnCursorCpp.Broadcast();
	auto interactions = GET_INTERFACES_RECURSIVE(InteractionTrigger, this);
	for (auto& interact : interactions)
	{
		if (interact.GetObject() == this)
		{
			continue;
		}
		interact->OnCursorOver_Implementation();
	}
}

bool AInteractiveObject::IsInteractable_Implementation() 
{ 
	return isInteractable; 
}
void AInteractiveObject::SetInteractable_Implementation(bool value) 
{
	isInteractable = value;

}