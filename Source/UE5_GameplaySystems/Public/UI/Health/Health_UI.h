// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Health_UI.generated.h"


/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UHealth_UI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* HeartsContainerBox;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMyLives_UI> HeartWidgetClass;

	TArray<class UMyLives_UI*> HeartWidgets;

	void InitializeHearts();

	int N = 0;
};
