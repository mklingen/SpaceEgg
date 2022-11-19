// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceEggGameMode.h"
#include "SpaceEggCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASpaceEggGameMode::ASpaceEggGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}