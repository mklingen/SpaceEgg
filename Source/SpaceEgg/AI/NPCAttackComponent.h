// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCAttackComponent.generated.h"

class ASpaceEggProjectile;
class UAnimMontage;

USTRUCT(BlueprintType)
struct FAttack
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = Attack)
		FString Name;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		FVector MuzzleOffset;
	
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		float MinProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		float MaxProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASpaceEggProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
		float AttackCooldownTime;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
		float Range;

	UPROPERTY(EditDefaultsOnly, Category = Attack)
		float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* AttackAnimation;

	UPROPERTY()
		float LastAttackTime = -1.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UNPCAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNPCAttackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Attack)
	TArray<FAttack> Attacks;
	

	UFUNCTION(BlueprintCallable)
		FAttack GetAttack(const FString& attackName) const;

	UFUNCTION(BlueprintCallable)
		bool HasAttack(const FString& attackName) const;

	UFUNCTION(BlueprintCallable)
		bool CanAttack(const FString& attackName, AActor* other) const;

	UFUNCTION(BlueprintCallable)
		bool DoAttack(const FString& attackName, AActor* other);
};
