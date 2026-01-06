// Copyright Epic Games, Inc. All Rights Reserved.
// GameMode responsible for defining default gameplay classes and rules

#include "UE5_GameplaySystemsGameMode.h"
#include "UE5_GameplaySystemsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE5_GameplaySystemsGameMode::AUE5_GameplaySystemsGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
