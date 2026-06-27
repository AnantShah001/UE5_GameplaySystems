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
#include "Data/Struct/ControlSpeed.h"


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

		// Moving
		// Bind movement input (WASD / joystick) to the Move() function
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE5_GameplayPlayerController::Move);

		// Jumping
		// Bind jump input: start jumping when key/button is pressed
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUE5_GameplayPlayerController::Jump);
		// Bind jump input: stop jumping when key/button is released
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUE5_GameplayPlayerController::StopJumping);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AUE5_GameplayPlayerController::Runing);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AUE5_GameplayPlayerController::Runing);

		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AUE5_GameplayPlayerController::Walking);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &AUE5_GameplayPlayerController::Walking);

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

void AUE5_GameplayPlayerController::Runing(const FInputActionValue& Value)
{
	bIsRuning = Value.Get<bool>();

	if (Value.Get<bool>()) bIsWalking = false;
}

void AUE5_GameplayPlayerController::Walking(const FInputActionValue& Value)
{
	bIsWalking = Value.Get<bool>();

	if (Value.Get<bool>()) bIsRuning = false;
}

// Called when movement input is received (WASD / joystick)
// Moves the character forward/back and right/left based on the input vector
void AUE5_GameplayPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	// Get the input vector (X = right/left, Y = forward/back)
	MovementVector = Value.Get<FVector2D>();

	if (MyCharacter != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		// Move the character forward/backward

		MyCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		// Move the character right/left
		MyCharacter->AddMovementInput(RightDirection, MovementVector.X);

		SmoothSpeed();
		MovementSpeed();
	}
}

void AUE5_GameplayPlayerController::SmoothSpeed()
{
	if (MyCharacter)
	{
		float AddSpeed = 5.f;
		float CurrentSpeed = MyCharacter->GetVelocity().Size() + AddSpeed;

		if (CurrentSpeed <= WalkSpeed)
		{
			MyCharacterMovement->MaxWalkSpeed = CurrentSpeed;
		}
		else
		{
			MyCharacterMovement->MaxWalkSpeed = WalkSpeed;
		}
	}
}

void AUE5_GameplayPlayerController::MovementSpeed()
{
	FName FindRow = MovementPosition();
	const UDataTable* SpeedTable = SpeedData.DataTable;

	if (SpeedTable != nullptr)
	{
		FControlSpeed* SelectRow = SpeedTable->FindRow<FControlSpeed>(FindRow, TEXT("Movement"));
		if (SelectRow != nullptr)
		{
			if (MovementVector.Y > 0.f)
			{
				WalkSpeed = SelectRow->Forward_Dir;
			}
			else
			{
				WalkSpeed = SelectRow->Other_Dir;
			}
		}
	}

}

FName AUE5_GameplayPlayerController::MovementPosition()
{
	FName Position;

	if (bIsRuning)
	{
		Position = FName(TEXT("Stand_Run"));
	}
	else if (bIsWalking)
	{
		Position = FName(TEXT("Stand_Walk"));
	}
	else
	{
		Position = FName(TEXT("Stand_Jog"));
	}
	
	UE_LOG(LogTemp, Display, TEXT("Current Speed : %s"), *Position.ToString());

	return Position;

}
