// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu_UI.generated.h"

class UButton;
class AUE5_GameplayPlayerController;

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UPauseMenu_UI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	TObjectPtr<UButton> PlayButton;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	TObjectPtr<UButton> ReStartButton;

	UPROPERTY(EditAnyWhere, meta = (BindWidget))
	TObjectPtr<UButton> QuitButton;

protected:

	virtual void NativeConstruct() override;	

private:

	UFUNCTION()
	void OnClickPlayButton();

	UFUNCTION()
	void OnClickReStartButton();

	UFUNCTION()
	void OnClickQuitButton();

	UPROPERTY()
	TObjectPtr<AUE5_GameplayPlayerController> PlayerController;
};
