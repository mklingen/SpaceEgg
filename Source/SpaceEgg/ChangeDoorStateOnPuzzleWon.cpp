// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeDoorStateOnPuzzleWon.h"
#include "Helpers/PrintHelper.h"
#include "Door.h"

// Sets default values for this component's properties
UChangeDoorStateOnPuzzleWon::UChangeDoorStateOnPuzzleWon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UChangeDoorStateOnPuzzleWon::BeginPlay()
{
	Super::BeginPlay();

	door = Cast<ADoor>(GetOwner());
	
}

void UChangeDoorStateOnPuzzleWon::OnPuzzleCompleted_Implementation()
{
	if (!door)
	{
		LOGE("No door attached.");
		return;
	}
	if (LockDoor)
	{
		door->LockDoor();
	}
	else
	{
		door->UnlockDoor();
	}

	if (OpenDoor)
	{
		door->OpenDoor();
	}
	else
	{
		door->CloseDoor();
	}
}



