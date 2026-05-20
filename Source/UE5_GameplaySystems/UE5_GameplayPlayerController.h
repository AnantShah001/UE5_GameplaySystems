// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UE5_GameplayPlayerController.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class UPauseMenu_UI;

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API AUE5_GameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUE5_GameplayPlayerController();

	UPROPERTY(EditAnywhere, Category = "Select UI")
	TSubclassOf<UPauseMenu_UI> PauseMenuRef;

	TObjectPtr<UPauseMenu_UI> PauseMenu;

	bool bIsPauseGame = false;

	void PauseMenuWidget();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseMenuAction;

private:

	bool bIsMainMenueWidget = false;
	
};
