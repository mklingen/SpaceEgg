// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerInteractionsComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "SpaceEgg/Helpers/PrintHelper.h"
#include "SpaceEgg/Helpers/ActorHelpers.h"
#include "SpaceEgg/Interfaces/InteractionTrigger.h"
#include "SpaceEgg/Helpers/DebugDrawHelpers.h"
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


TScriptInterface<IInteractionTrigger> UTriggerInteractionsComponent::SphereSweepOrNull(float radius, const FVector& start, const FVector& end, AActor*& actorHit)
{
	FHitResult hit;
	if (UKismetSystemLibrary::SphereTraceSingle(this, start, end, radius, UEngineTypes::ConvertToTraceType(CollisionTraceChannel), false, TArray<AActor*>(), EnableDebugDraw ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, hit, true, FLinearColor::Red, FLinearColor::Green, 0.1f))
	{
		auto actor = hit.GetActor();
		if (actor)
		{
			if (EnableDebugDraw)
			{
				DRAW_STRING("%s", hit.ImpactPoint, FColor::Red, 0.1f, true, *(actor->GetActorNameOrLabel()));
			}
			TArray<TScriptInterface<IInteractionTrigger>> interfaces;
			UActorHelpers::FindActorOrComponentInterfacesRecursive<IInteractionTrigger>(UInteractionTrigger::StaticClass(), actor, interfaces);
			if (interfaces.IsEmpty())
			{
				if (EnableDebugDraw)
				{
					DrawDebugLine(GetWorld(), start, hit.Location, FColor::Red, false, 0.1f);
				}
				return false;
			}
			for (TScriptInterface<IInteractionTrigger> obj : interfaces)
			{
				if (IInteractionTrigger::Execute_IsInteractable(obj.GetObject()))
				{
					actorHit = actor;
					return obj;
				}
			}
			if (EnableDebugDraw)
			{
				DrawDebugLine(GetWorld(), start, hit.Location, FColor::Green, false, 0.1f);
			}
			return nullptr;
		}
	}
	return nullptr;
}

TScriptInterface<IInteractionTrigger> UTriggerInteractionsComponent::GetObjectUnderCursorOrNull(AActor*& actorHit)
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
	auto little_sweep = SphereSweepOrNull(10, start, end, actorHit);
	if (little_sweep)
	{
		return little_sweep;
	}
	actorHit = nullptr;
	return SphereSweepOrNull(InteractSize, start, end, actorHit);
}

bool UTriggerInteractionsComponent::Trigger()
{
	AActor* hit = nullptr;
	auto obj = GetObjectUnderCursorOrNull(hit);
	if (!obj)
	{
		return false;
	}
	IInteractionTrigger::Execute_OnInteraction(obj.GetObject());
	LOGI("Interacting with %s", *(obj.GetObject()->GetName()));
	return true;
}

