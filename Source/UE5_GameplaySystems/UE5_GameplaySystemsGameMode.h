// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UE5_GameplaySystemsGameMode.generated.h"



/**
 * GameMode class responsible for defining default gameplay classes and rules.
 * Sets the default pawn to the project’s character blueprint.
 */
UCLASS(minimalapi) // Macro to make the class visible to Unreal Editor; minimalapi speeds up compilation for this module
class AUE5_GameplaySystemsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUE5_GameplaySystemsGameMode();
};



