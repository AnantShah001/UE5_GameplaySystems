// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyLives_UI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeartAnimFinished, UMyLives_UI*, WidgetToDelete);

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

	// Create the instance of the delegate
	UPROPERTY(BlueprintAssignable)
	FOnHeartAnimFinished OnAnimationFinishedDelegate;

	void PlayHeartLoseAnim();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HeartNumber;
	
	void SetNumber(int Num);

	UPROPERTY(EditAnyWhere, Category = "Effects")
	class USoundBase* HeartLoseSound;

protected:
	UFUNCTION()
	void OnHeartLoseAnimFinished();
};