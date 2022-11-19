// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "DoorStateListener.h"

#include "Helpers/ActorHelpers.h"

void ADoor::ConnectListeners()
{
	TArray<TScriptInterface<IDoorStateListener> > listeners;
	UActorHelpers::FindActorOrComponentInterfacesRecursive<IDoorStateListener>(UDoorStateListener::StaticClass(), this, listeners);
	for (TScriptInterface<IDoorStateListener> listener : listeners)
	{
		FScriptDelegate delegate;
		delegate.BindUFunction(listener.GetObject(), TEXT("OnDoorStateChanged"));
		OnStateChange.Add(delegate);
	}
}

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	ConnectListeners();
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (GetState())
	{
		case DoorState::ClosedAndLocked:
		case DoorState::ClosedAndUnlocked:
		case DoorState::Open:
		{
			if (DebugAutoAnimate)
			{
				if (GetState() == DoorState::Open)
				{
					CloseDoor();
				}
				else
				{
					UnlockDoor();
					OpenDoor();
				}
			}
			break;
		}
		case DoorState::Opening: 
		{
			if (GetWorld()->GetTimeSeconds() > timeToStopMoving)
			{
				SetState(DoorState::Open);
			}
			break;
		}
		case DoorState::Closing:
		{
			if (GetWorld()->GetTimeSeconds() > timeToStopMoving)
			{
				SetState(DoorState::ClosedAndUnlocked);
			}
			break;
		}
	}

}

bool ADoor::LockDoor()
{
	if (CanLock()) 
	{
		SetState(ClosedAndLocked);
		return true;
	}
	return false;
}

bool ADoor::UnlockDoor()
{
	if (CanUnLock())
	{
		SetState(ClosedAndUnlocked);
		return true;
	}
	return false;
}

bool ADoor::OpenDoor()
{
	if (CanOpen())
	{
		timeStartedMoving = GetWorld()->GetTimeSeconds();
		timeToStopMoving = timeStartedMoving + AnimationTime;
		SetState(Opening);
		return true;
	}
	return false;
}

bool ADoor::CloseDoor()
{
	if (CanClose())
	{
		timeStartedMoving = GetWorld()->GetTimeSeconds();
		timeToStopMoving = timeStartedMoving + AnimationTime;
		SetState(Closing);
		return true;
	}
	return false;
}


TEnumAsByte<DoorState> ADoor::GetState() const
{
	return state;
}

bool ADoor::CanLock() const
{
	return GetState() == ClosedAndUnlocked;
}

bool ADoor::CanUnLock() const
{
	return GetState() == ClosedAndLocked;
}

bool ADoor::CanClose() const
{
	return (state != ClosedAndLocked && state != ClosedAndUnlocked && state != Closing);
}

bool ADoor::CanOpen() const
{
	return (state != Open && state != Opening);
}
void ADoor::SetState(TEnumAsByte<DoorState> _state)
{
	state = _state;
	OnStateChange.Broadcast(this);
}


