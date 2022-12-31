// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceEgg/Interfaces/DamageCallback.h"
#include "SpaceEgg/Helpers/SFXHelpers.h"
#include "DamageSFXComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UDamageSFXComponent : public UActorComponent, public IDamageCallback
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageSFXComponent();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SFX")
		FSFXHelpers Effects;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TakeDamage_Implementation(const FHitResult& hitResult, float Damage, const UDamageType* DamageType);

};
