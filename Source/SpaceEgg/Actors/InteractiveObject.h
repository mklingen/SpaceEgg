// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceEgg/Interfaces/InteractionTrigger.h"
#include "InteractiveObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCursorOver);
DECLARE_MULTICAST_DELEGATE(FOnInteractCpp);
DECLARE_MULTICAST_DELEGATE(FOnCursorOverCpp);

UCLASS()
class SPACEEGG_API AInteractiveObject : public AActor, public IInteractionTrigger
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractiveObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
		int activationCount = 0;

	UPROPERTY()
		bool toggleState = 0;

	UPROPERTY()
		bool isInteractable = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
		int GetActivationCount() const { return activationCount; }

	UFUNCTION(BlueprintCallable)
		bool GetToggleState() const { return toggleState; }

	UFUNCTION(BlueprintCallable)
		void SetToggleState(bool val) { toggleState = val; }


	UPROPERTY(BlueprintAssignable, Category = "Interaction")
		FOnInteract OnInteract;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
		FOnCursorOver OnCursor;

		FOnInteractCpp OnInteractCpp;
		FOnCursorOverCpp OnCursorCpp;

	virtual void OnInteraction_Implementation() override;
	virtual void OnCursorOver_Implementation() override;
	virtual bool IsInteractable_Implementation() override;
	virtual void SetInteractable_Implementation(bool value) override;
};
