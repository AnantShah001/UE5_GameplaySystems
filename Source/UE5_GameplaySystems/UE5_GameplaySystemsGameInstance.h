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
	int Gold;
	int HeartLifeLine;

	void RespawnPlayer(FVector Location);
	void SetScore(int Score);

	void HealthLifeLine(int LifeLine);
};
