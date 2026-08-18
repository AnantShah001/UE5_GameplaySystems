// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Android_ScreenControls_UI.generated.h"

class AUE5_GameplaySystemsCharacter;
class UButton;
class AUE5_GameplayPlayerController;

/**
 *
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UAndroid_ScreenControls_UI : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UButton> JumpButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> PauseButton;

protected:
	
	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void JumpButtonPressed();

	UFUNCTION()
	void PauseButtonPressed();

	UPROPERTY()
	TObjectPtr<AUE5_GameplaySystemsCharacter> MyCharacter;

	UPROPERTY()
	TObjectPtr<AUE5_GameplayPlayerController> MyController;

};
