// Fill out your copyright notice in the Description page of Project Settings.


#include "BinarySwitchboxPuzzle.h"
#include "SpaceEgg/Interfaces/Toggleable.h"
#include "SpaceEgg/Helpers/ActorHelpers.h"
#include "SpaceEgg/Helpers/PrintHelper.h"
#include "SpaceEgg/Helpers/DebugDrawHelpers.h"
#include "SpaceEgg/Actors/Puzzles/BinaryPuzzle.h"
#include "SpaceEgg/Actors/Doors/Door.h"
#include "SpaceEgg/Actors/InteractiveObject.h"
#include "SpaceEgg/Actors/Puzzles/PuzzleCompletedCallback.h"

namespace
{
	enum class IndicatorState
	{
		Off = 0,
		OnGood = 1,
		OnBad = 2
	};
	void SetStateOnToggles(AActor* actor, IndicatorState state)
	{
		TArray<TScriptInterface<IToggleable> > toggles;
		UActorHelpers::FindActorOrComponentInterfacesRecursive<IToggleable>(UToggleable::StaticClass(), actor, toggles);
		for (auto& toggle : toggles)
		{
			toggle->SetState_Implementation(static_cast<int>(state));
		}
	}
}

// Sets default values
ABinarySwitchboxPuzzle::ABinarySwitchboxPuzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Puzzle = CreateDefaultSubobject<UBinaryPuzzle>(TEXT("Puzzle"));
}

void ABinarySwitchboxPuzzle::DestroyChildren() 
{
	// Destroy the door and the handle.
	auto destroyChild = [](AActor* child)
	{
		if (child) 
		{ 
			child->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); 
			child->Destroy(); 
		}
	};
	destroyChild(Door);
	destroyChild(DoorHandle);

	// Destroy the buttons.
	for (auto& buttons : { Buttons1, Buttons2 })
	{
		for (auto& element : buttons)
		{
			destroyChild(element);
		}
	}
	Buttons1.Empty();
	Buttons2.Empty();

	// Destroy the success indicators.
	for (auto& element : SuccessIndicators)
	{
		destroyChild(element);
	}

	SuccessIndicators.Empty();
	TArray<AActor*> children;
	GetAttachedActors(children, true, true);
	for (auto& child : children)
	{
		child->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		child->Destroy();
	}
	Door = nullptr;
	DoorHandle = nullptr;
}

// Called when the object is moved or if parameters have changed.
void ABinarySwitchboxPuzzle::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (PropertyChanged)
	{
		//RecreateChildren();
	}
}


void ABinarySwitchboxPuzzle::CreateDoor()
{
	Door = UActorHelpers::CreateAndAttach<ADoor>(DoorClass, this, DoorRelativeSpawnLocation, DoorRelativeSpawnRotation);
	DoorHandle = UActorHelpers::CreateAndAttach<AInteractiveObject>(DoorHandleClass, this, DoorHandleRelativeSpawnLocation, DoorHandleRelativeSpawnRotation);
	// If the door and the doorhandle both exist, attach the door handle to the door and have it respond
	// by opening and closing the door.
	if (Door && DoorHandle)
	{
		DoorHandle->OnInteractCpp.AddLambda([this]()
			{
				if (Door->CanOpen() && !IsPuzzleComplete) 
				{
					Door->SetActorEnableCollision(false);
					Door->OpenDoor();
				}
				else if (Door->CanClose())
				{
					Door->SetActorEnableCollision(true);
					Door->CloseDoor();
				}
			});
	}
}

// Create the buttons, success indicators, etc.
void ABinarySwitchboxPuzzle::CreateButtons()
{
	// Create buttons.
	for (int i = 0; i < Puzzle->GetSize(); i++) 
	{
		Buttons1.Add(Cast<AInteractiveObject>(CreateColumnElement(Column1, i, ControllerButtonClass)));
		Buttons2.Add(Cast<AInteractiveObject>(CreateColumnElement(Column2, i, ControllerButtonClass)));
		AActor* success = CreateColumnElement(SuccessColumn, i, SuccessClass);
		if (success)
		{
			SetStateOnToggles(success, Puzzle->CheckSolution(i) ? IndicatorState::OnGood : IndicatorState::OnBad);
			SuccessIndicators.Add(success);
		}
	}

	const auto buttons = std::vector<TArray<AInteractiveObject*>*>{ &Buttons1, &Buttons2 };
	// Set the initial state of the buttons.
	for (int i = 0; i < Puzzle->GetSize(); i++) 
	{
		for (int column = 0; column < 2; column++) 
		{
			AInteractiveObject* button = (*buttons[column])[i];
			if (button)
			{
				// Set the button's initial state.
				button->SetToggleState(Puzzle->GetState(column, i));
				SetStateOnToggles(button, (Puzzle->GetState(column, i) ? IndicatorState::OnGood : IndicatorState::Off));
				int buttonIdx = i;
				int columnIdx = column;
				// Attach lambda to the button so that when it is pressed, we get a callback.
				button->OnInteractCpp.AddLambda([this, buttonIdx, columnIdx]()
					{
						this->OnButtonPushed(columnIdx, buttonIdx);
					});
			}
			else
			{
				LOGE("Was not able to create button.");
			}
		}
	}
	IsFlashingSolution = true;
	SolutionIdxToFlash = 0;
	TimeLastChanged = GetWorld()->GetTimeSeconds();
}

// Creates a button or indicator for the given column and index, returning the element.
AActor* ABinarySwitchboxPuzzle::CreateColumnElement(const FColumn& col, int idx, TSubclassOf<AActor> actor) {
	FVector origin
		= col.Origin + col.Direction * idx * col.Spacing;
	FVector originEwrtWorld = this->GetActorTransform().TransformPosition(origin);
	FActorSpawnParameters params;
	params.bNoFail = true;
	params.Owner = this;
	AActor* element = GetWorld()->SpawnActorAbsolute<AActor>(actor, FTransform(GetActorRotation(), originEwrtWorld, FVector3d(1, 1, 1)), params);
	if (!element) 
	{
		LOGE("Failed to create column element.");
		return nullptr;
	}
	element->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	return element;
}


void ABinarySwitchboxPuzzle::RecreateChildren()
{
	DestroyChildren();
	CreateButtons();
	CreateDoor();
	PropertyChanged = false;
}

void ABinarySwitchboxPuzzle::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	PropertyChanged = true;
}

// Called when the game starts or when spawned.
void ABinarySwitchboxPuzzle::BeginPlay()
{
	Super::BeginPlay();
	RecreateChildren();
}

// Called every frame.
void ABinarySwitchboxPuzzle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// If the puzzle is won, set the success indicators to on.
	if (Puzzle->IsPuzzleWon_Implementation()) 
	{
		for (int i = 0; i < Puzzle->GetSize(); i++)
		{
			if (SuccessIndicators[i])
			{
				SetStateOnToggles(SuccessIndicators[i], IndicatorState::OnGood);
			}
		}
	} // Otherwise, turn off the success indicators after a period.
	else if (IsFlashingSolution && GetWorld()->GetTimeSeconds() - TimeLastChanged > TimeToFlashSolution)
	{
		for (int i = 0; i < Puzzle->GetSize(); i++)
		{
			if (SuccessIndicators[i])
			{
				SetStateOnToggles(SuccessIndicators[i], IndicatorState::Off);
			}
		}
		IsFlashingSolution = false;
	}
}

void ABinarySwitchboxPuzzle::OnButtonPushed(int col, int i)
{
	AInteractiveObject* interactive = col == 0x0 ? Buttons1[i] : Buttons2[i];
	if (interactive)
	{
		Puzzle->Set(col, i, interactive->GetToggleState());
	}
	if (SuccessIndicators[i])
	{
		SetStateOnToggles(SuccessIndicators[i], Puzzle->CheckSolution(i) ? IndicatorState::OnGood : IndicatorState::OnBad);
	}
	if (DebugDraw)
	{
		for (int k = 0; k < Puzzle->GetSize(); k++)
		{
			DRAW_STRING("%d", Buttons1[k]->GetActorLocation(), FColor::Red, 1.0f, true, Puzzle->GetState1(k));
			DRAW_STRING("%d", Buttons2[k]->GetActorLocation(), FColor::Green, 1.0f, true, Puzzle->GetState2(k));
			DRAW_STRING("%d", SuccessIndicators[k]->GetActorLocation(), FColor::Blue, 1.0f, true, Puzzle->CheckSolution(k));
		}
	}
	IsFlashingSolution = true;
	TimeLastChanged = GetWorld()->GetTimeSeconds();
	SolutionIdxToFlash = i;
	if (Puzzle->IsPuzzleWon_Implementation())
	{
		// Tell all the callbacks in the room that they should receive an OnPuzzleCompleted from this puzzle.
		if (RoomActor.IsValid())
		{
			auto callbacks = GET_INTERFACES_RECURSIVE(PuzzleCompletedCallback, RoomActor.Get());
			for (auto& callback : callbacks)
			{
				FScriptDelegate d;
				d.BindUFunction(callback.GetObject(), TEXT("OnPuzzleCompleted"));
				OnPuzzleComplete.Add(d);
			}
		}
		IsPuzzleComplete = true;
		OnPuzzleComplete.Broadcast();
		if (Door)
		{
			Door->SetActorEnableCollision(true);
			Door->CloseDoor();
		}
		if (DoorHandle)
		{
			DoorHandle->SetInteractable_Implementation(false);
		}
	}
}