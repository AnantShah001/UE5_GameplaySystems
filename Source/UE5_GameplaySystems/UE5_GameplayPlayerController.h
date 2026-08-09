// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UE5_GameplayPlayerController.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class UPauseMenu_UI;
class AUE5_GameplaySystemsCharacter;
class UCharacterMovementComponent;


/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API AUE5_GameplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUE5_GameplayPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Select UI")
	TSubclassOf<UPauseMenu_UI> PauseMenuRef;

	UPROPERTY()
	TObjectPtr<UPauseMenu_UI> PauseMenu;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr< AUE5_GameplaySystemsCharacter> MyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UCharacterMovementComponent> MyCharacterMovement;

	bool bIsPauseGame = false;

	void PauseMenuWidget();



protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseMenuAction;

private:

	bool bIsMainMenueWidget = false;
};
