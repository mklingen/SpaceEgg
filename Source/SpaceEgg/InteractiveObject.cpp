// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveObject.h"

// Sets default values
AInteractiveObject::AInteractiveObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInteractiveObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractiveObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveObject::OnInteraction_Implementation()
{
	OnInteract.Broadcast();
}

void AInteractiveObject::OnCursorOver_Implementation()
{
	OnCursor.Broadcast();
}