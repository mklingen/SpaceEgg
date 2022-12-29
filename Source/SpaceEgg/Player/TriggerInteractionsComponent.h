// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpaceEgg/Interfaces/InteractionTrigger.h"
#include "TriggerInteractionsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UTriggerInteractionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerInteractionsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
		float MaxRange = 1500;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
		float InteractSize = 20;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
		TEnumAsByte<ECollisionChannel> CollisionTraceChannel = ECollisionChannel::ECC_Visibility;
	
	UFUNCTION()
		TScriptInterface<IInteractionTrigger> GetObjectUnderCursorOrNull();

	UFUNCTION()
		TScriptInterface<IInteractionTrigger> SphereSweepOrNull(float radius, const FVector& start, const FVector& end);
	UFUNCTION()
		bool Trigger();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Debug")
		bool EnableDebugDraw = false;
};
