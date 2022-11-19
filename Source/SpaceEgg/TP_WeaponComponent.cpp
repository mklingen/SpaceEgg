// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "SpaceEggCharacter.h"
#include "SpaceEggProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	PrimaryComponentTick.bCanEverTick = true;
}


// Called every frame
void UTP_WeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (IsTriggerHeldDown)
	{
		switch (ShootType)
		{
			case ChargeShot:
			{
				Charge = FMath::Clamp(MaxCharge * ((GetWorld()->TimeSeconds - TimeTriggerPressed) / ChargeTime), 0, MaxCharge);
				break;
			}
			case AutoShot:
			{
				float time = GetWorld()->TimeSeconds;
				if (time - TimeOfLastShot > AutoShootSpeed)
				{
					Fire();
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void UTP_WeaponComponent::Fire()
{
	if(Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	if (Ammo <= 0)
	{
		Charge = 0;
		return;
	}

	Ammo--;
	TimeOfLastShot = GetWorld()->TimeSeconds;
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			// Spawn the projectile at the muzzle
			ASpaceEggProjectile* projectile = World->SpawnActor<ASpaceEggProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			if (ShootType == ChargeShot && projectile)
			{
				FVector vel = projectile->GetProjectileMovement()->Velocity;
				vel.Normalize();
				vel *= FMath::Clamp((Charge / MaxCharge) * MaxProjectileSpeed, MinProjectileSpeed, MaxProjectileSpeed);
				projectile->GetProjectileMovement()->Velocity = vel;
			}
			else if (projectile)
			{
				FVector vel = projectile->GetProjectileMovement()->Velocity;
				vel.Normalize();
				vel *= FMath::FRandRange(MinProjectileSpeed, MaxProjectileSpeed);
				projectile->GetProjectileMovement()->Velocity = vel;
			}
		}
	}
	Charge = 0;
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(Character != nullptr)
	{
		// Unregister from the OnUseItem Event
		Character->OnShoot.RemoveDynamic(this, &UTP_WeaponComponent::TriggerHeldDown);
		Character->OnShootReleased.RemoveDynamic(this, &UTP_WeaponComponent::TriggerReleased);
	}
}

void UTP_WeaponComponent::AttachWeapon(ASpaceEggCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if(Character != nullptr)
	{
		// Attach the weapon to the First Person Character
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		GetOwner()->AttachToComponent(Character->GetMesh1P(),AttachmentRules, FName(TEXT("GripPoint")));

		// Register so that Fire is called every time the character tries to use the item being held
		Character->OnShoot.AddDynamic(this, &UTP_WeaponComponent::TriggerHeldDown);
		Character->OnShootReleased.AddDynamic(this, &UTP_WeaponComponent::TriggerReleased);
	}
}

void UTP_WeaponComponent::TriggerHeldDown()
{
	IsTriggerHeldDown = true;
	TimeTriggerPressed = GetWorld()->GetTimeSeconds();
	switch (ShootType)
	{
		case SingleShot:
		{
			Fire();
			break;
		}
		default:
		{
			break;
		}
	}
}


void UTP_WeaponComponent::TriggerReleased()
{
	IsTriggerHeldDown = false;
	switch (ShootType)
	{
		case ChargeShot:
		{
			Fire();
			break;
		}
	}
}

