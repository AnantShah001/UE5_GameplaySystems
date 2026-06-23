// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UE5_GameplaySystemsHUD.h"
#include "UE5_Gameplay_AndroidScreen_HUD.generated.h"

class UAndroid_ScreenControls_UI;

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API AUE5_Gameplay_AndroidScreen_HUD : public AUE5_GameplaySystemsHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UAndroid_ScreenControls_UI> AndroidWidgetClass;

	UPROPERTY()
	TObjectPtr<UAndroid_ScreenControls_UI> AndroidControls;
	
};
