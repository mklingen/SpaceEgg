// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorChangerComponent.h"

#include "SpaceEgg/Helpers/PrintHelper.h"

UColorChangerComponent::UColorChangerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UColorChangerComponent::BeginPlay()
{
	Super::BeginPlay();
	mesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	Update();
}

int UColorChangerComponent::GetState_Implementation() const 
{
	return State;
}

void UColorChangerComponent::SetState_Implementation(int value)
{
	if (State != value)
	{
		State = value;
		Update();
	}
}

void UColorChangerComponent::Update()
{
	if (!mesh)
	{
		return;
	}
	if (!Materials.IsValidIndex(State))
	{
		LOGE("State %d invalid. No Materials for that state.", State);
		return;
	}
	mesh->SetMaterial(MaterialIndex, Materials[State]);
}




