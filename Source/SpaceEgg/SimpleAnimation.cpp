// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleAnimation.h"
#include "Helpers/PrintHelper.h"

// Sets default values for this component's properties
USimpleAnimation::USimpleAnimation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USimpleAnimation::BeginPlay()
{
	Super::BeginPlay();
	TransformA = GetOwner()->GetActorTransform();
	TransformB = TransformB * TransformA;
}


// Called every frame
void USimpleAnimation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!MovementCurve)
	{
		LOGE("NO movement curve.");
		return;
	}
	float time = GetWorld()->GetTimeSeconds();
	float alpha = FMath::Clamp((time - timeStartedAnimating) / AnimationTime, 0.0f, 1.0f);
	switch (state) {
		case StoppedAtA:
		{
			GetOwner()->SetActorTransform(TransformA);
			break;
		}
		case StoppedAtB:
		{
			GetOwner()->SetActorTransform(TransformB);
			break;
		}
		case MovingFromAToB:
		{
			if (time > targetAnimationTime) {
				SetState(SimpleAnimationState::StoppedAtB);
				break;
			}
			float curveValue = MovementCurve->GetFloatValue(alpha);
			FVector interpPos = FMath::Lerp(TransformA.GetLocation(), TransformB.GetLocation(), curveValue);
			FQuat interpRot = FQuat::Slerp(TransformA.GetRotation(), TransformB.GetRotation(), curveValue).GetNormalized();
			FRotator rot(interpRot);
			GetOwner()->SetActorLocationAndRotation(interpPos, interpRot);
			break;
		}
		case MovingFromBToA:
		{
			if (time > targetAnimationTime) {
				SetState(SimpleAnimationState::StoppedAtA);
				break;
			}
			float curveValue = MovementCurve->GetFloatValue(1.0f - alpha);
			FVector interpPos = FMath::Lerp(TransformA.GetLocation(), TransformB.GetLocation(), curveValue);
			FQuat interpRot = FQuat::Slerp(TransformA.GetRotation(), TransformB.GetRotation(), curveValue).GetNormalized();
			GetOwner()->SetActorLocationAndRotation(interpPos, interpRot);
			break;
		}
	}
}



void USimpleAnimation::Animate(bool moveFromAToB, float _animationTime)
{
	AnimationTime = _animationTime;
	targetAnimationTime = GetWorld()->GetTimeSeconds() + AnimationTime;
	timeStartedAnimating = GetWorld()->GetTimeSeconds();
	if (moveFromAToB)
	{
		SetState(SimpleAnimationState::MovingFromAToB);
	}
	else
	{
		SetState(SimpleAnimationState::MovingFromBToA);
	}
}

void USimpleAnimation::SetState(TEnumAsByte<SimpleAnimationState> _state)
{
	state = _state;
}
