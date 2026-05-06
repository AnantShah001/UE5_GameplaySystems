// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Health_UI.generated.h"

class UHorizontalBox;
class UMyLives_UI;
/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UHealth_UI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HeartsContainerBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMyLives_UI> HeartWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UMyLives_UI>> HeartWidgets;

	UPROPERTY()
	TObjectPtr<UMyLives_UI> Heart;

	void InitializeHearts();
	void PlayRemoveHeartAnim();

protected:
	UFUNCTION()
	void OnHeartAnimationFinished(UMyLives_UI* WidgetToRemove);
};
