// Anant Shah All Rights Reserved
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreUI.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UScoreUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldScore;

	void SetScore(int Score);
};
