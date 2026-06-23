// Anant Shah All Rights Reserved


#include "UE5_GameplayPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Menu/PauseMenu_UI.h"
#include "UE5_GameplaySystems/UE5_GameplaySystemsCharacter.h"


AUE5_GameplayPlayerController::AUE5_GameplayPlayerController()
{
	//
}

void AUE5_GameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	MyCharacter = Cast< AUE5_GameplaySystemsCharacter>(GetPawn());
}

void AUE5_GameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Triggered, this,	&AUE5_GameplayPlayerController::PauseMenuWidget);

		// Jumping
		// Bind jump input: start jumping when key/button is pressed
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUE5_GameplayPlayerController::Jump);
		// Bind jump input: stop jumping when key/button is released
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUE5_GameplayPlayerController::StopJumping);
	}
}

void AUE5_GameplayPlayerController::PauseMenuWidget()
{
	if (!PauseMenuRef) return;

	if(!PauseMenu)
	{
		PauseMenu = CreateWidget<UPauseMenu_UI>(this, PauseMenuRef);
		PauseMenu->AddToViewport();
		
		// Set Input Mode
		FInputModeGameAndUI InputModeData;

		InputModeData.SetWidgetToFocus(PauseMenu->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputModeData.SetHideCursorDuringCapture(true);

		SetInputMode(InputModeData);
		SetShowMouseCursor(true);
		bIsPauseGame = true;

		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UE_LOG(LogTemp, Warning, TEXT("Paus Add To viewPort"));
	}

	else
	{
		PauseMenu->RemoveFromParent();
		PauseMenu = nullptr;

		UGameplayStatics::SetGamePaused(GetWorld(), false);

		// Set Input Mode
		FInputModeGameOnly GameOnlyData;
		SetInputMode(GameOnlyData);

		SetShowMouseCursor(false);
		bIsPauseGame = false;
		UE_LOG(LogTemp, Warning, TEXT("Paus Remove To viewPort"));
	}
}

void AUE5_GameplayPlayerController::Jump()
{
	if (MyCharacter)
	{
		MyCharacter->Jump();
	}
}

void AUE5_GameplayPlayerController::StopJumping()
{
	if (MyCharacter)
	{
		MyCharacter->StopJumping();
	}
}