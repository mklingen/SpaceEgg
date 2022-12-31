// Fill out your copyright notice in the Description page of Project Settings.


#include "CauseDamageOnHitComponent.h"
#include "SpaceEgg/Helpers/ActorHelpers.h"
#include "SpaceEgg/Helpers/PrintHelper.h"
#include "SpaceEgg/Components/HealthComponent.h"

// Sets default values for this component's properties
UCauseDamageOnHitComponent::UCauseDamageOnHitComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCauseDamageOnHitComponent::BeginPlay()
{
	Super::BeginPlay();
	UPrimitiveComponent* primitive = GetOwner()->FindComponentByClass<UPrimitiveComponent>();
	if (!primitive)
	{
		LOGE("%s has no primitive.", *(GetOwner()->GetActorNameOrLabel()));
	}
	else
	{
		primitive->OnComponentHit.AddDynamic(this, &UCauseDamageOnHitComponent::OnHit);
	}
}


void UCauseDamageOnHitComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Find the health components of any actors we hit.
	TArray<UHealthComponent*> healths = UActorHelpers::FindComponentsRecursive<UHealthComponent>(OtherActor);
	bool hitAny = healths.Num() > 0;
	for (UHealthComponent* health : healths)
	{
		// Cause damage to them.
		health->TakeDamage(Hit, OtherActor, DamageAmount, DamageType, nullptr, GetOwner());
	}
	// If hitting any other health components, and our own health exists, destroy ourself by
	// setting health to zero.
	if (hitAny && ZeroOwnHealthOnHit)
	{
		UHealthComponent* health = UActorHelpers::FindComponentRecursive<UHealthComponent>(GetOwner());
		if (!health) 
		{
			LOGW("Damage causer had no health, but ZeroOwnHealthOnHit set to true. Just destroying.");
			GetOwner()->Destroy();
		}
		else
		{
			health->TakeDamage(Hit, GetOwner(), health->GetHealth(), nullptr, nullptr, GetOwner());
		}
	}
	else if (ZeroOwnHealthIfHitAnything)
	{
		UHealthComponent* health = UActorHelpers::FindComponentRecursive<UHealthComponent>(GetOwner());
		if (!health)
		{
			LOGW("Damage causer had no health, but ZeroOwnHealthOnHit set to true. Just destroying.");
			GetOwner()->Destroy();
		}
		else
		{
			health->TakeDamage(Hit, GetOwner(), health->GetHealth(), nullptr, nullptr, GetOwner());
		}
	}
}

