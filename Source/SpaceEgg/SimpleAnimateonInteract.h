// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleAnimation.h"
#include "InteractionTrigger.h"
#include "SimpleAnimateonInteract.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API USimpleAnimateonInteract : public USimpleAnimation, public IInteractionTrigger
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleAnimateonInteract();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnInteraction_Implementation() override;
};
