// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CauseDamageOnHitComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UCauseDamageOnHitComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCauseDamageOnHitComponent();

	// If true, the actor will be destroyed when it causes damage to another
	// actor. Note that a health component must exist for this to make sense.
	// If a health component doesn't exist, the actor will instead be immediately
	// destroyed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool ZeroOwnHealthOnHit = true;

	// Amount of damage to apply.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		float DamageAmount = 1.0f;

	// Type of damage to apply to the thing we just hit.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UDamageType* DamageType = nullptr;

	// If true, the actor will be destroyed even if it doesn't cause damage to another actor.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool ZeroOwnHealthIfHitAnything = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called when another actor's component begins overlapping this component.
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
