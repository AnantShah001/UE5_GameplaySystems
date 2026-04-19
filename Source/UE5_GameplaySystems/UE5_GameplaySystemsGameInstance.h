// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UE5_GameplaySystemsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UUE5_GameplaySystemsGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UUE5_GameplaySystemsGameInstance();
	
	FVector RespawnLocation;

	void RespawnPlayer(FVector Location);
	
};
