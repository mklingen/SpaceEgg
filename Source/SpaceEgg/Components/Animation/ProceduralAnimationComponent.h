// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProceduralAnimationComponent.generated.h"


UENUM()
enum ProceduralGaitPhase
{
	Rest UMETA(DisplayName = "At Rest"),
	Left UMETA(DisplayName = "Left Foot"),
	Right UMETA(DisplayName = "Right Foot")
};

UENUM()
enum ProceduralStancePhase
{
	Down,
	Up
};

USTRUCT(Blueprintable)
struct FFootState
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
		FTransform PrevTargetEwrtWorld;
	UPROPERTY(BlueprintReadWrite)
		FTransform CurrentTargetEwrtWorld;
	UPROPERTY(BlueprintReadWrite)
		FTransform NextTargetEwrtWorld;
	UPROPERTY(BlueprintReadWrite)
		FTransform CurrentSwingTarget;
};

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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Gait")
		TEnumAsByte <ProceduralGaitPhase> GaitPhase = ProceduralGaitPhase::Left;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Gait")
		TEnumAsByte<ProceduralStancePhase> Stance = ProceduralStancePhase::Down;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float TimeLastPhaseStarted = -1.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float Phase = 0.0f;
	
	UFUNCTION()
		void UpdateNextStance(ProceduralGaitPhase forPhase);

	void EvaluateSwingPhase(FFootState* footState);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		FVector DesiredVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		float DesiredYawVelocity = 0.0f;

	UFUNCTION(BlueprintCallable)
		void Stop();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float GaitTime= 0.25f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float NominalStanceHeight = -50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float SwingLiftHeight = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float StanceWidth = 25.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float ArmWidth = 25.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float ArmSwingDist = 25.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IK")
		bool ControlRightHandTarget = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IK")
		bool ControlLeftHandTarget = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IK")
		FTransform RightHandTargetEwrtWorld;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IK")
		FTransform LeftHandTargetEwrtWorld;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IK")
		FFootState RightFootState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IK")
		FFootState LeftFootState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "IK")
		FVector GazeTargetEwrtWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UCurveFloat* FootSwingCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UCurveFloat* ArmSwingCurve;

};
