// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionTrigger.h"
#include "InteractiveObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCursorOver);

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
		FOnInteract OnInteract;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
		FOnCursorOver OnCursor;

	virtual void OnInteraction_Implementation() override;
	virtual void OnCursorOver_Implementation() override;
};
