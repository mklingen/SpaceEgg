// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceEggNPC.h"

#include "SpaceEgg/Components/DamageSFXComponent.h"
#include "SpaceEgg/Components/DeathSFXComponent.h"
#include "SpaceEgg/Components/HealthComponent.h"
#include "SpaceEgg/AI/NPCAttackComponent.h"

ASpaceEggNPC::ASpaceEggNPC()
{
	PrimaryActorTick.bCanEverTick = true;
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Attack = CreateDefaultSubobject<UNPCAttackComponent>(TEXT("Attack"));
	DamageSFX = CreateDefaultSubobject<UDamageSFXComponent>(TEXT("Damage sfx"));
	DeathSFX = CreateDefaultSubobject<UDeathSFXComponent>(TEXT("Death sfx"));
}

void ASpaceEggNPC::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpaceEggNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpaceEggNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

