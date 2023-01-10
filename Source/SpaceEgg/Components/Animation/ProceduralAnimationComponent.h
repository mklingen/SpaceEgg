// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralAnimationComponent.generated.h"

class AController;
class AAIController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UProceduralAnimationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProceduralAnimationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
		AController* controller = nullptr;

	UPROPERTY()
		AAIController* aiController = nullptr;

	float prevYaw = 0.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		FVector DesiredVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		float DesiredYawVelocity = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		FVector GazeControl = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		float GazeSpeed = 4.0;
};
