// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAttackComponent.h"

#include "SpaceEgg/Components/HealthComponent.h"
#include "SpaceEgg/Actors/SpaceEggProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
UNPCAttackComponent::UNPCAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}


// Called when the game starts
void UNPCAttackComponent::BeginPlay()
{
	Super::BeginPlay();


	
}

FAttack UNPCAttackComponent::GetAttack(const FString& attackName) const
{
	if (!HasAttack(attackName)) {
		return FAttack();
	}

	for (const FAttack& attack : Attacks)
	{
		if (attack.Name == attackName)
		{
			return attack;
		}
	}
	return FAttack();
}

bool UNPCAttackComponent::HasAttack(const FString& attackName) const 
{
	for (const FAttack& attack : Attacks)
	{
		if (attack.Name == attackName)
		{
			return true;
		}
	}
	return false;
}

bool UNPCAttackComponent::CanAttack(const FString& attackName, AActor* other) const 
{
	if (!other)
	{
		return false;
	}
	float time = GetWorld()->GetTimeSeconds();
	for (const FAttack& attack : Attacks)
	{
		if (attack.Name == attackName)
		{
			float dt = time - attack.LastAttackTime;
			if (dt < attack.AttackCooldownTime)
			{
				return false;
			}
			float dist = FVector::Dist(GetOwner()->GetActorLocation(), other->GetActorLocation());
			if (dist > attack.Range)
			{
				return false;
			}
			return true;
		}
	}
	return false;
}

bool UNPCAttackComponent::DoAttack(const FString& attackName, AActor* other) 
{
	if (!CanAttack(attackName, other))
	{
		return false;
	}
	// TODO, raycasting, maybe?
	for (FAttack& attack : Attacks)
	{
		if (attack.Name != attackName)
		{
			continue;
		}
		attack.LastAttackTime = GetWorld()->GetTimeSeconds();
		TimeOfLastAttack = attack.LastAttackTime;
		UHealthComponent* health = other->FindComponentByClass<UHealthComponent>();
		if (health && attack.MeleeDamage > 0)
		{
			FHitResult hit;
			hit.Location = GetOwner()->GetActorLocation();
			hit.Normal = (other->GetActorLocation() - hit.Location).GetSafeNormal();
			health->TakeDamage(hit, other, attack.MeleeDamage, nullptr, nullptr, GetOwner());
			attack.Effects.Play(this, hit.Location, GetOwner()->GetActorRotation());
		}

		if (attack.ProjectileClass)
		{
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorRotation().RotateVector(attack.MuzzleOffset);
			// TODO: add some inaccuracy and lobbing.
			FVector delta = other->GetActorLocation() - GetOwner()->GetActorLocation();
			delta.Normalize();
			const FRotator SpawnRotation = delta.ToOrientationRotator();
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

 			ASpaceEggProjectile* projectile = GetWorld()->SpawnActor<ASpaceEggProjectile>(attack.ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (projectile)
			{
				FVector vel = projectile->GetProjectileMovement()->Velocity;
				vel.Normalize();
				vel *= FMath::FRandRange(attack.MinProjectileSpeed, attack.MaxProjectileSpeed);
				vel += FVector::UpVector * attack.ProjectileArcSpeed;
				projectile->GetProjectileMovement()->Velocity = vel;
			}
			attack.Effects.Play(this, SpawnLocation, SpawnRotation);
		}

	}
	return true;
}

bool UNPCAttackComponent::DidAttackRecently(float timeThreshold) const
{
	float time = GetWorld()->GetTimeSeconds();
	return time - TimeOfLastAttack < timeThreshold;
}
