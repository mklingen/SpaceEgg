// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleAnimation.h"
#include "Toggleable.h"
#include "SimpleAnimateonToggle.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SPACEEGG_API USimpleAnimateOnToggle : public USimpleAnimation, public IToggleable
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USimpleAnimateOnToggle();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	int toggleValue = 0;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual int GetState_Implementation() const override;
	virtual void SetState_Implementation(int value) override;


};
