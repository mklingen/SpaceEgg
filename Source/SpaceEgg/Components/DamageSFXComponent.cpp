// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageSFXComponent.h"
#include "SpaceEgg/Helpers/SFXHelpers.h"


// Sets default values for this component's properties
UDamageSFXComponent::UDamageSFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Effects = CreateDefaultSubobject<USFXHelpers>(TEXT("Effects"));
}


// Called when the game starts
void UDamageSFXComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UDamageSFXComponent::TakeDamage_Implementation(const FHitResult& hitResult, float Damage, const UDamageType* DamageType)
{
	if (Effects)
	{
		AActor* owner = GetOwner();
		Effects->Play(hitResult.Location, hitResult.Normal.ToOrientationRotator(), owner->GetActorScale());
	}
}