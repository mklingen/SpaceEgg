// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimpleAnimation.generated.h"

UENUM()
enum SimpleAnimationState
{
	StoppedAtA    UMETA(DisplayName = "Stopped at A"),
	MovingFromAToB    UMETA(DisplayName = "Moving From A to B"),
	MovingFromBToA    UMETA(DisplayName = "Moving From A to B"),
	StoppedAtB UMETA(DisplayName = "Stopped at B")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEEGG_API USimpleAnimation : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleAnimation();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		TEnumAsByte<SimpleAnimationState> state;

	void SetState(TEnumAsByte<SimpleAnimationState> state);

	float targetAnimationTime = -1.0f;
	float timeStartedAnimating = -1.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation", meta = (MakeEditWidget))
		FTransform TransformA;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (MakeEditWidget))
		FTransform TransformB;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UCurveFloat* MovementCurve;
	
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<SimpleAnimationState> GetAnimationState() const {
		return state;
	}

	UFUNCTION(BlueprintCallable)
	void Animate(bool moveFromAToB, float animationTime);

	UFUNCTION(BlueprintCallable)
	bool IsAnimationDone() const {
		return state == SimpleAnimationState::StoppedAtA || state == SimpleAnimationState::StoppedAtB;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float AnimationTime = 1.0f;
};
