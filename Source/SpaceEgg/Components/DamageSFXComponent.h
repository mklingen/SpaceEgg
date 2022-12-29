// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceEgg/Interfaces/DamageCallback.h"
#include "DamageSFXComponent.generated.h"

class USFXHelpers;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UDamageSFXComponent : public UActorComponent, public IDamageCallback
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageSFXComponent();
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = "SFX")
		USFXHelpers* Effects = nullptr;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TakeDamage_Implementation(const FHitResult& hitResult, float Damage, const UDamageType* DamageType);

};
