// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceEgg/Interfaces/DeathCallback.h"
#include "DeathSFXComponent.generated.h"

class USoundCue; 
class UNiagaraSystem;
class USoundAttenuation;
class USFXHelpers;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UDeathSFXComponent : public UActorComponent, public IDeathCallback
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDeathSFXComponent();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Instanced, Category = "SFX")
		USFXHelpers* Effects = nullptr;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void OnDied_Implementation();
		
};
