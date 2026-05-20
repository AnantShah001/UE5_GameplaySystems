// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UE5_GameplaySystemsHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API AUE5_GameplaySystemsHUD : public AHUD
{
	GENERATED_BODY()

public:

	AUE5_GameplaySystemsHUD();

protected:

	virtual void BeginPlay() override;

};
