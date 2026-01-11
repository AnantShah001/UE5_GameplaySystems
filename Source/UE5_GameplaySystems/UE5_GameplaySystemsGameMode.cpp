// Copyright Epic Games, Inc. All Rights Reserved.
// GameMode responsible for defining default gameplay classes and rules

#include "UE5_GameplaySystemsGameMode.h"
#include "UE5_GameplaySystemsCharacter.h"
#include "UObject/ConstructorHelpers.h"

// The Constructor: This runs when the GameMode is first initialized
AUE5_GameplaySystemsGameMode::AUE5_GameplaySystemsGameMode()
{
	/**
	 * ConstructorHelpers::FClassFinder:
	 * This searches the project's Content folder for a specific Blueprint class.
	 * <APawn> defines the type of class we are looking for.
	 * The TEXT path is the location of your character Blueprint in the Unreal Editor.
	 * set default pawn class to our Blueprinted character
	 */
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	//Check if the Blueprint was actually found at the path provided
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
