// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralAnimationComponent.h"

#include "AIController.h"

// Sets default values for this component's properties
UProceduralAnimationComponent::UProceduralAnimationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UProceduralAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* owner = GetOwner();
	if (owner)
	{
		APawn* pawn = Cast<APawn>(owner);
		if (pawn)
		{
			controller = pawn->GetController();
			aiController = Cast<AAIController>(controller);
		}
	}
	prevYaw = controller->GetControlRotation().Yaw;

}

void UProceduralAnimationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (controller)
	{
		DesiredVelocity = controller->GetPawn()->GetVelocity();
		DesiredYawVelocity = (controller->GetControlRotation().Yaw - prevYaw) / DeltaTime;
		prevYaw = controller->GetControlRotation().Yaw;
		if (aiController)
		{
			GazeControl = FMath::Lerp(GazeControl, aiController->GetFocalPoint(), FMath::Clamp(GazeSpeed * DeltaTime, 0.0f, 1.0f));
		}
	}
}
