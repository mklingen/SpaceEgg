// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "SpaceEgg/Helpers/ActorHelpers.h"
#include "SpaceEgg/Interfaces/DamageCallback.h"
#include "SpaceEgg/Interfaces/DeathCallback.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	FScriptDelegate callback;
	callback.BindUFunction(this, TEXT("TakeDamage"));
	GetOwner()->OnTakeAnyDamage.Add(callback);

	auto damageCallbacks = GET_INTERFACES_RECURSIVE(DamageCallback, GetOwner());
	for (auto damageCallback : damageCallbacks)
	{
		FScriptDelegate callbackDelegate;
		callbackDelegate.BindUFunction(damageCallback.GetObject(), TEXT("TakeDamage"));
		OnDamaged.Add(callbackDelegate);
	}

	auto deathCallbacks = GET_INTERFACES_RECURSIVE(DeathCallback, GetOwner());
	for (auto deathCallback : deathCallbacks)
	{
		FScriptDelegate callbackDelegate;
		callbackDelegate.BindUFunction(deathCallback.GetObject(), TEXT("OnDied"));
		OnDeath.Add(callbackDelegate);
	}

	GetOwner()->OnDestroyed.AddDynamic(this, &UHealthComponent::OnOwnerDestroyed);
}

void UHealthComponent::OnOwnerDestroyed(AActor* owner)
{
	if (Health <= 0 || !owner)
	{
		return;
	}
	FHitResult hitResult;
	hitResult.Location = owner->GetActorLocation();
	TakeDamage(hitResult, owner, Health + 1, nullptr, nullptr, owner);
}

void UHealthComponent::TakeDamage(const FHitResult& hitResult, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser)
{
	if (Health <= 0)
	{
		return;
	}

	OnDamagedCpp.Broadcast(hitResult, Damage, DamageType);
	OnDamaged.Broadcast(hitResult, Damage, DamageType);
	
	Health -= Damage;
	if (Health <= 0)
	{
		Health = 0;
		OnDeathCpp.Broadcast();
		OnDeath.Broadcast();
		if (DestroyOwnerOnDeath)
		{
			GetOwner()->SetHidden(true);
			GetOwner()->SetLifeSpan(TimeToRemainAliveAfterZeroHp);
		}
	}
}

float UHealthComponent::GetHealth() const
{
	return Health;
}

