// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDiedEvent);
DECLARE_MULTICAST_DELEGATE(FDiedEventCpp);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamagedEvent, const FHitResult&, HitResult, float, DamageAmount, const UDamageType*, DamageType);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDamagedEventCpp, const FHitResult&, float, const UDamageType*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, Category = "Health")
		float Health = 0;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Health")
		float MaxHealth = 0;
	
	UFUNCTION()
		void OnOwnerDestroyed(AActor* owner);

public:	
	virtual void TakeDamage(const FHitResult& hitResult, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy, AActor* DamageCauser);
	
	UFUNCTION(BlueprintCallable)
		float GetHealth() const;

	UPROPERTY(BlueprintAssignable, Category = "Death")
		FDiedEvent OnDeath;

	FDiedEventCpp OnDeathCpp;

	UPROPERTY(BlueprintAssignable, Category = "Damaged")
		FOnDamagedEvent OnDamaged;

	FOnDamagedEventCpp OnDamagedCpp;

	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Death")
		bool DestroyOwnerOnDeath = true;
	UPROPERTY(BlueprintReadwrite, EditAnywhere, Category = "Death")
		float TimeToRemainAliveAfterZeroHp = 0.1f;
};
