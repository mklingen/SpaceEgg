// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProceduralWalkAnimation.generated.h"

class UProceduralAnimationComponent;

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

USTRUCT(Blueprintable)
struct FSimpleBoneChain {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName RootBoneName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName EndBoneName;
	FBoneReference RootBone;
	FBoneReference EndBone;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float NominalLength = 0.0f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		FTransform NominalPoseEndEwrtRoot;

	void Initialize(USkeletalMeshComponent* skeleton);
	FTransform GetRootTBoneRecursive(int boneIdx, int rootIdx, USkeletalMeshComponent* component) const;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API UProceduralWalkAnimation : public UAnimInstance
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProceduralWalkAnimation();

protected:

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

	UPROPERTY()
		UProceduralAnimationComponent* animationComponent = nullptr;

	void EvaluateSwingPhase(FFootState* footState);
	FTransform EvaluateHandSwing(const FTransform& worldTformActor, const FTransform& nominalEwrtActor, float phase);
	void DoFootIK();
	void DoHandIK();
	void DoBodyBounce();
	float GetRelativeBodyBounce();
	FTransform ComputeNominalHandPose(const FSimpleBoneChain& chain, const FRotator& armNominalRot, USkeletalMeshComponent* mesh);
	void ComputeNominalHandPoses(USkeletalMeshComponent* mesh);

	FTransform GetActorTransformOrIdentity() const;
	ProceduralGaitPhase GaitPhaseOnStop = ProceduralGaitPhase::Rest;

public:	
	// Called when the game starts
	virtual void NativeInitializeAnimation() override;
	// Called every frame
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DebugDraw")
		bool DebugDrawNominalShoulderPose = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DebugDraw")
		bool DebugDrawNominalHandPose = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		FVector DesiredVelocity = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controls")
		float DesiredYawVelocity = 0.0f;

	UFUNCTION(BlueprintCallable)
		void Stop();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float GaitTime= 0.25f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Gait")
		float NominalStanceHeight = -50.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float SwingLiftHeight = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float StanceWidth = 25.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		FRotator ArmNominalSwingRotLeft;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		FRotator ArmNominalSwingRotRight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		FVector ArmNominalSwingTransLeft;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		FVector ArmNominalSwingTransRight;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float ArmSwingDist = 25.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gait")
		float BodyBounceHeight = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hand IK")
		bool ControlRightHandTarget = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hand IK")
		bool ControlLeftHandTarget = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gaze IK")
		FVector GazePointEwrtBodyDuringWalk = FVector(500, 0, 90);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gaze IK")
		bool ControlGazeTarget = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hand IK")
		FTransform LeftHandNominalEwrtBody;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Hand IK")
		FTransform RightHandNominalEwrtBody;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hand IK")
		FTransform RightHandTargetEwrtWorld;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Hand IK")
		FTransform LeftHandTargetEwrtWorld;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Foot IK")
		FFootState RightFootState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Foot IK")
		FFootState LeftFootState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Gaze IK")
		FVector GazeTargetEwrtWorld;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Body IK")
		FTransform BodyTargetEwrtWorld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UCurveFloat* FootSwingCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UCurveFloat* ArmSwingCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UCurveFloat* BodyBounceCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand IK")
		FSimpleBoneChain RightArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hand IK")
		FSimpleBoneChain LeftArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Foot IK")
		FSimpleBoneChain RightLeg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Foot IK")
		FSimpleBoneChain LeftLeg;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gait")
		bool IsStopped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gait")
		float StoppedGaitTime = 0.5f;


};