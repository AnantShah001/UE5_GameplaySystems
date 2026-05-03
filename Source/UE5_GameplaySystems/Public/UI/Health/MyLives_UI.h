// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyLives_UI.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UMyLives_UI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation* HeartLoseAnim;

	void PlayHeartLoseAnim();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HeartNumber;
	
	void SetNumber(int Num);
};