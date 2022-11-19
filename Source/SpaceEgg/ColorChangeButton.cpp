// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorChangeButton.h"

// Sets default values
AColorChangeButton::AColorChangeButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

// Called when the game starts or when spawned
void AColorChangeButton::BeginPlay()
{
	Super::BeginPlay();
	FScriptDelegate d;
	d.BindUFunction(this, TEXT("Toggle"));
	OnInteract.Add(d);
	SetState(state);
}

void AColorChangeButton::Toggle()
{
	if (state == ButtonState::Off)
	{
		SetState(ButtonState::On);
	}
	else
	{
		SetState(ButtonState::Off);
	}
}



void AColorChangeButton::SetState(TEnumAsByte<ButtonState> buttonState)
{
	state = buttonState;
	if (state == ButtonState::Off && OffMaterial)
	{
		mesh->SetMaterial(MaterialIndex, OffMaterial);
	}
	else if (state == ButtonState::On && OnMaterial)
	{
		mesh->SetMaterial(MaterialIndex, OnMaterial);
	}
}