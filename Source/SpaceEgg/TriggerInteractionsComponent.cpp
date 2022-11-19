// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerInteractionsComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers/PrintHelper.h"
#include "Helpers/ActorHelpers.h"
#include "InteractionTrigger.h"
#include "Helpers/DebugDrawHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UTriggerInteractionsComponent::UTriggerInteractionsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


}


// Called when the game starts
void UTriggerInteractionsComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UTriggerInteractionsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

TScriptInterface<IInteractionTrigger> UTriggerInteractionsComponent::GetObjectUnderCursorOrNull()
{
	FVector loc;
	FRotator rot;
	auto cameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (!cameraManager)
	{
		return nullptr;
	}
	cameraManager->GetCameraViewPoint(loc, rot);
	FHitResult hit;
	FVector start = loc;
	FVector end = loc + rot.RotateVector(FVector3d::ForwardVector) * MaxRange;
	if (UKismetSystemLibrary::SphereTraceSingle(this, start, end, InteractSize, UEngineTypes::ConvertToTraceType(CollisionTraceChannel), false, TArray<AActor*>(), EnableDebugDraw ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, hit, true))
	{
		auto actor = hit.GetActor();
		if (actor)
		{
			if (EnableDebugDraw)
			{
				DRAW_STRING("%s", hit.ImpactPoint, FColor::Red, 1.0f, true, *(actor->GetActorNameOrLabel()));
			}
			TArray<TScriptInterface<IInteractionTrigger>> interfaces;
			UActorHelpers::FindActorOrComponentInterfacesRecursive<IInteractionTrigger>(UInteractionTrigger::StaticClass(), actor, interfaces);
			if (interfaces.IsEmpty())
			{
				if (EnableDebugDraw)
				{
					DrawDebugLine(GetWorld(), loc, hit.Location, FColor::Red, false, 1.0f);
				}
				return false;
			}
			for (TScriptInterface<IInteractionTrigger> obj : interfaces)
			{
				return obj;
			}
			if (EnableDebugDraw)
			{
				DrawDebugLine(GetWorld(), loc, hit.Location, FColor::Green, false, 1.0f);
			}
			return nullptr;
		}
	}
	return nullptr;
}

bool UTriggerInteractionsComponent::Trigger()
{
	auto obj = GetObjectUnderCursorOrNull();
	if (!obj)
	{
		return false;
	}
	IInteractionTrigger::Execute_OnInteraction(obj.GetObject());
	return true;
}

