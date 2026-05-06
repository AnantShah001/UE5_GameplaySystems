// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyLives_UI.generated.h"

class UMyLives_UI;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeartAnimFinished, UMyLives_UI*, WidgetToDelete);

class UWidgetAnimation;
class USoundBase;

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UMyLives_UI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HeartLoseAnim;

	// Create the instance of the delegate
	UPROPERTY(BlueprintAssignable)
	FOnHeartAnimFinished OnAnimationFinishedDelegate;

	void PlayHeartLoseAnim();

	UPROPERTY(EditAnyWhere, Category = "Effects")
	TObjectPtr<USoundBase> HeartLoseSound;

protected:
	UFUNCTION()
	void OnHeartLoseAnimFinished();
};