// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceEggNPC.h"

#include "SpaceEgg/Components/DamageSFXComponent.h"
#include "SpaceEgg/Components/DeathSFXComponent.h"
#include "SpaceEgg/Components/HealthComponent.h"
#include "SpaceEgg/AI/NPCAttackComponent.h"
#include "SpaceEgg/Player/SpaceEggCharacter.h"
#include "Perception/PawnSensingComponent.h"

ASpaceEggNPC::ASpaceEggNPC()
{
	PrimaryActorTick.bCanEverTick = true;
	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	Attack = CreateDefaultSubobject<UNPCAttackComponent>(TEXT("Attack"));
	DamageSFX = CreateDefaultSubobject<UDamageSFXComponent>(TEXT("Damage sfx"));
	DeathSFX = CreateDefaultSubobject<UDeathSFXComponent>(TEXT("Death sfx"));
	Sensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensor"));
}

void ASpaceEggNPC::OnSeePawn(APawn* pawn)
{
	LastSeenPawn = pawn;
	ASpaceEggCharacter* player = Cast<ASpaceEggCharacter>(pawn);
	if (player)
	{
		LastSeenPlayer = player;
	}
}

void ASpaceEggNPC::BeginPlay()
{
	Super::BeginPlay();
	if (Sensor)
	{
		Sensor->OnSeePawn.AddDynamic(this, &ASpaceEggNPC::OnSeePawn);
	}
}

bool ASpaceEggNPC::CanSeeCharacter() const
{
	if (!LastSeenPlayer)
	{
		return false;
	}
	if (!Sensor)
	{
		return false;
	}
	return Sensor->CouldSeePawn(LastSeenPlayer);
}

void ASpaceEggNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpaceEggNPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APawn* ASpaceEggNPC::GetLastSeenPawn() const
{
	return LastSeenPawn;
}

ASpaceEggCharacter* ASpaceEggNPC::GetLastSeenCharacter() const
{
	return LastSeenPlayer;
}

