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
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"


AUE5_GameplayPlayerController::AUE5_GameplayPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUE5_GameplayPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);	

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
	if (MyCharacter)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
	}

}

void AUE5_GameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(PauseMenuAction, ETriggerEvent::Triggered, this,	&AUE5_GameplayPlayerController::PauseMenuWidget);

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
