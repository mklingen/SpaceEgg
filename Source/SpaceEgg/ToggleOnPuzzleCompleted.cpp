// Fill out your copyright notice in the Description page of Project Settings.

#include "ToggleOnPuzzleCompleted.h"

#include "Helpers/ActorHelpers.h"
#include "Toggleable.h"

void UToggleOnPuzzleCompleted::ToggleAll(bool completed)
{
	TArray<TScriptInterface<IToggleable>> toggles;
	UActorHelpers::FindActorOrComponentInterfacesRecursive(UToggleable::StaticClass(), GetOwner(), toggles);
	for (auto& toggle : toggles)
	{
		toggle->SetState_Implementation(completed ? StateOnPuzzleComplete : StateOnPuzzleIncomplete);
	}
}

// Sets default values for this component's properties
UToggleOnPuzzleCompleted::UToggleOnPuzzleCompleted()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}


// Called when the game starts
void UToggleOnPuzzleCompleted::BeginPlay()
{
	Super::BeginPlay();

	ToggleAll(false);
}

void UToggleOnPuzzleCompleted::OnPuzzleCompleted_Implementation()
{
	ToggleAll(true);
}

