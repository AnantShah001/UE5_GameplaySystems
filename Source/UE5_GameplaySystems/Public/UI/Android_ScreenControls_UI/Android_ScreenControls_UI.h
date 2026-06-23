// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Android_ScreenControls_UI.generated.h"

class AUE5_GameplaySystemsCharacter;
class UButton;

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

protected:
	
	virtual void NativeConstruct() override;

private:

	UFUNCTION()
	void JumpButtonPressed();

	UPROPERTY()
	TObjectPtr<AUE5_GameplaySystemsCharacter> MyCharacter;

};
