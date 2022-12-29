// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_WeaponComponent.generated.h"

class ASpaceEggCharacter;

UENUM()
enum WeaponShootType
{
	SingleShot     UMETA(DisplayName = "Single Shot"),
	ChargeShot      UMETA(DisplayName = "Charge Shot"),
	AutoShot   UMETA(DisplayName = "AutoShot")
};

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UTP_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ASpaceEggProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** Sets default values for this component's properties */
	UTP_WeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category="Weapon")
		void AttachWeapon(ASpaceEggCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void TriggerHeldDown();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void TriggerReleased();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Weapon")
		void Fire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
		int Ammo = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float ChargeTime = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float MaxCharge = 100;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
		float Charge = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float AutoShootSpeed = 0.5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float MinProjectileSpeed = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float MaxProjectileSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FString AmmoType = TEXT("Bolts");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TEnumAsByte<WeaponShootType> ShootType;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
		bool IsTriggerHeldDown = false;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
		float TimeTriggerPressed = 0.0;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
		float TimeOfLastShot = 0.0;
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		

private:
	/** The Character holding this weapon*/
	ASpaceEggCharacter* Character;
};
