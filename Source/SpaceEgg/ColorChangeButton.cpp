// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorChangeButton.h"

// Sets default values
AColorChangeButton::AColorChangeButton()
{
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	colorChanger = CreateDefaultSubobject<UColorChangerComponent>(TEXT("Color Changer"));
}

// Called when the game starts or when spawned
void AColorChangeButton::BeginPlay()
{
	Super::BeginPlay();
	FScriptDelegate d;
	d.BindUFunction(this, TEXT("Toggle"));
	OnInteract.Add(d);
	SetButtonState(state);
}

void AColorChangeButton::Toggle()
{
	if (state == ButtonState::Off)
	{
		SetButtonState(ButtonState::On);
	}
	else
	{
		SetButtonState(ButtonState::Off);
	}
}



void AColorChangeButton::SetButtonState(TEnumAsByte<ButtonState> buttonState)
{
	state = buttonState;
	if (colorChanger) 
	{
		colorChanger->SetState_Implementation(static_cast<int>(state));
	}
}

int AColorChangeButton::GetState_Implementation() const 
{
	return colorChanger ? colorChanger->GetState_Implementation() : 0;
}
void AColorChangeButton::SetState_Implementation(int value) 
{
	if (colorChanger)
	{
		colorChanger->SetState_Implementation(value);
	}
}