// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceEgg/Interfaces/DeathCallback.h"
#include "SpaceEgg/Helpers/SFXHelpers.h"
#include "DeathSFXComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UDeathSFXComponent : public UActorComponent, public IDeathCallback
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDeathSFXComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SFX")
		FSFXHelpers Effects;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	virtual void OnDied_Implementation();
		
};
