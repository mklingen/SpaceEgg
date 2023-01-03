// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralAnimationComponent.h"

// Sets default values for this component's properties
UProceduralAnimationComponent::UProceduralAnimationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UProceduralAnimationComponent::BeginPlay()
{
	Super::BeginPlay();

	TimeLastPhaseStarted = GetWorld()->GetTimeSeconds();
	Phase = 0.0f;
	UpdateNextStance(ProceduralGaitPhase::Left);
	UpdateNextStance(ProceduralGaitPhase::Right);
	GazeTargetEwrtWorld = GetOwner()->GetActorTransform().TransformPosition(FVector::UnitX() * 500 + FVector::UnitZ() * 90);
	LeftHandTargetEwrtWorld = GetOwner()->GetActorTransform();
	RightHandTargetEwrtWorld = GetOwner()->GetActorTransform();
}


// Called every frame
void UProceduralAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float t = GetWorld()->GetTimeSeconds();
	Phase = (t - TimeLastPhaseStarted) / GaitTime;
	GazeTargetEwrtWorld = GetOwner()->GetActorTransform().TransformPosition(FVector::UnitX() * 500 + FVector::UnitZ() * 90);
	switch (GaitPhase)
	{
		case ProceduralGaitPhase::Rest: 
		{
			break;
		}
		case ProceduralGaitPhase::Left:
		{
			if (Phase > 1.0f) 
			{
				TimeLastPhaseStarted = t;
				GaitPhase = ProceduralGaitPhase::Right;
				Stance = ProceduralStancePhase::Down;
				break;
			}
			if (Stance == ProceduralStancePhase::Down)
			{
				UpdateNextStance(GaitPhase);
				Stance = ProceduralStancePhase::Up;
				break;
			}
			EvaluateSwingPhase(&LeftFootState);
			break;
		}
		case ProceduralGaitPhase::Right: 
		{
			if (Phase > 1.0f)
			{
				TimeLastPhaseStarted = t;
				GaitPhase = ProceduralGaitPhase::Left;
				Stance = ProceduralStancePhase::Down;
				break;
			}
			if (Stance == ProceduralStancePhase::Down)
			{
				UpdateNextStance(GaitPhase);
				Stance = ProceduralStancePhase::Up;
				break;
			}
			EvaluateSwingPhase(&RightFootState);
			break;
		}
	}
}

void UProceduralAnimationComponent::EvaluateSwingPhase(FFootState* footState)
{
	float tSwing = Phase;
	if (FootSwingCurve)
	{
		tSwing = FootSwingCurve->GetFloatValue(tSwing);
	}
	FVector interpPos = footState->CurrentSwingTarget.GetLocation();
	FQuat interpRot = footState->CurrentSwingTarget.GetRotation();
	if (tSwing < 0.5)
	{
		interpPos = FMath::Lerp(footState->PrevTargetEwrtWorld.GetLocation(), footState->CurrentSwingTarget.GetLocation(), tSwing * 2);
		interpRot = FQuat::Slerp(footState->PrevTargetEwrtWorld.GetRotation(), footState->CurrentSwingTarget.GetRotation(), tSwing * 2).GetNormalized();
	}
	else
	{
		interpPos = FMath::Lerp(footState->CurrentSwingTarget.GetLocation(), footState->NextTargetEwrtWorld.GetLocation(), tSwing * 2 - 1.0);
		interpRot = FQuat::Slerp(footState->CurrentSwingTarget.GetRotation(), footState->NextTargetEwrtWorld.GetRotation(), tSwing * 2 - 1.0).GetNormalized();
	}
	footState->CurrentTargetEwrtWorld = FTransform(interpRot, interpPos);
}

void UProceduralAnimationComponent::UpdateNextStance(ProceduralGaitPhase forPhase)
{
	AActor* owner = GetOwner();
	if (!owner)
	{
		return;
	}
	// TODO: raycasting to terrain and such.
	const FTransform tCenter = owner->GetActorTransform();
	FTransform tProjected(tCenter.Rotator().Add(0, DesiredYawVelocity * GaitTime, 0), tCenter.GetTranslation() + DesiredVelocity * GaitTime);
	FTransform tHalfway(tCenter.Rotator().Add(0, DesiredYawVelocity * GaitTime * 0.5f, 0), tCenter.GetTranslation() + DesiredVelocity * GaitTime * 0.5f);
	const FVector& upDirectionEwrtBody = tCenter.TransformVector(FVector::UpVector);
	const FVector sideDirectionEwrtBody = forPhase == ProceduralGaitPhase::Left ? -1.0f * FVector::UnitY() :  FVector::UnitY();
	FTransform nextFootPose(tProjected.Rotator(), 
							tProjected.TransformPosition(sideDirectionEwrtBody * StanceWidth + upDirectionEwrtBody * NominalStanceHeight));
	FTransform swingTarget(tHalfway.Rotator(),
		tHalfway.TransformPosition(sideDirectionEwrtBody * StanceWidth + upDirectionEwrtBody * NominalStanceHeight + upDirectionEwrtBody * SwingLiftHeight));

	auto updateState = [&](FFootState& state)
	{
		state.CurrentSwingTarget = swingTarget;
		state.PrevTargetEwrtWorld = state.NextTargetEwrtWorld;
		state.NextTargetEwrtWorld = nextFootPose;
	};
	if (forPhase == ProceduralGaitPhase::Left)
	{
		updateState(LeftFootState);
	}
	else
	{
		updateState(RightFootState);
	}
}

void UProceduralAnimationComponent::Stop()
{
	Phase = ProceduralGaitPhase::Rest;
}

