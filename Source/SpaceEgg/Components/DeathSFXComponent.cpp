// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathSFXComponent.h"
#include "SpaceEgg/Helpers/SFXHelpers.h"

// Sets default values for this component's properties
UDeathSFXComponent::UDeathSFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	Effects = CreateDefaultSubobject<USFXHelpers>(TEXT("Effects"));
}


// Called when the game starts
void UDeathSFXComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UDeathSFXComponent::OnDied_Implementation()
{
	if (Effects)
	{
		AActor* owner = GetOwner();
		Effects->Play(owner->GetActorLocation(), owner->GetActorRotation(), owner->GetActorScale());
	}
}
