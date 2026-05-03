// Copyright Epic Games, Inc. All Rights Reserved.
// Player character using UE's third-person movement template

#include "UE5_GameplaySystemsCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
// Allows binding Enhanced Input actions to character functions.
#include "EnhancedInputComponent.h"
// Manages input mappings for the local player; used to add the Input Mapping Context at runtime.
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/UserWidget.h"
#include "UI/Death.h"
#include "UI/ScoreUI.h"
#include "GameFramework/InputSettings.h"
#include "UE5_GameplaySystems/UE5_GameplaySystemsGameInstance.h"
#include "UI/Health/Health_UI.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUE5_GameplaySystemsCharacter

AUE5_GameplaySystemsCharacter::AUE5_GameplaySystemsCharacter()
{
	// Set size for the collision capsule (width = 42, height = 96)
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create camera boom (spring arm) to position camera behind the character
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

// Called when the game starts or when the character is spawned
void AUE5_GameplaySystemsCharacter::BeginPlay()
{
	// Call the base class BeginPlay (important for inherited functionality)
	Super::BeginPlay();

	MyGameInstance = GetGameInstance<UUE5_GameplaySystemsGameInstance>();
	if (!MyGameInstance)return;
	//Add Input Mapping Context
	// Get the PlayerController controlling this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get the Enhanced Input subsystem for the local player
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add this character's Input Mapping Context so the Enhanced Input system knows which actions are active
			// Priority 0 = default priority
			Subsystem->AddMappingContext(DefaultMappingContext, 0);

		}

		ScoreWidget = CreateWidget<UScoreUI>(GetWorld(), ScoreRef);
		if (ScoreWidget)
		{
			ScoreWidget->AddToViewport();
		}
		HealthUI = CreateWidget<UHealth_UI>(GetWorld(), Health_UI_Ref);
		if (HealthUI)
		{
			HealthUI->AddToViewport();
			HealthUI->InitializeHearts();
		}
	}
}

void AUE5_GameplaySystemsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFallCameraActive)
	{
		// 1. Get positions
		FVector CameraLoc = FollowCamera->GetComponentLocation();
		FVector TargetLoc = GetMesh()->GetComponentLocation();

		// 2. Calculate the "Look At" rotation
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(CameraLoc, TargetLoc);

		// 3. Apply rotation (use RInterpTo for smoothness)
		FRotator CurrentRot = FollowCamera->GetComponentRotation();
		FRotator SmoothedRot = FMath::RInterpTo(CurrentRot, LookAtRot, DeltaTime, 5.0f);

		FollowCamera->SetWorldRotation(SmoothedRot);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
// Binds Enhanced Input actions to character movement functions.
// This is called once when the Pawn is possessed by a Controller.
void AUE5_GameplaySystemsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		// Bind jump input: start jumping when key/button is pressed
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		// Bind jump input: stop jumping when key/button is released
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		// Bind movement input (WASD / joystick) to the Move() function
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE5_GameplaySystemsCharacter::Move);

		// Looking
		// Bind look input (mouse / right joystick) to the Look() function
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE5_GameplaySystemsCharacter::Look);

		EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Triggered, this, &AUE5_GameplaySystemsCharacter::DebugActionPressed);

		// Mobile Touch
		EnableTouchScreenMovement(EnhancedInputComponent);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

// Called when movement input is received (WASD / joystick)
// Moves the character forward/back and right/left based on the input vector
void AUE5_GameplaySystemsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	// Get the input vector (X = right/left, Y = forward/back)
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		// Move the character forward/backward
		AddMovementInput(ForwardDirection, MovementVector.Y);
		// Move the character right/left
		AddMovementInput(RightDirection, MovementVector.X);
	}
}


// Called when look input is received (mouse / right joystick)
// Rotates the camera/controller based on input
void AUE5_GameplaySystemsCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	// Get the look input vector (X = yaw/left-right, Y = pitch/up-down)
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		// Rotate the controller yaw (left/right)
		AddControllerYawInput(LookAxisVector.X);
		// Rotate the controller pitch (up/down)
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AUE5_GameplaySystemsCharacter::DebugActionPressed()
{
	UE_LOG(LogTemp, Display, TEXT("Debug Input Pressed"));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Cyan, TEXT("Debug Input Triggered"));
}

void AUE5_GameplaySystemsCharacter::HandleDeath()
{
	bIsRespawning = true;
	GetMesh()->SetSimulatePhysics(true);
	DisableInput(nullptr);
	bFallCameraActive = true;
	GetWorldTimerManager().SetTimer(DeathWidgetTimer, this, &AUE5_GameplaySystemsCharacter::DeathWidgetAnimation, 2.0f, false);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AUE5_GameplaySystemsCharacter::RespawnPlayer, 3.0f, false);
}

void AUE5_GameplaySystemsCharacter::RespawnPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawn Player"));
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	if (!MyGameInstance->RespawnLocation.IsZero())
	{
		//Capsule Component
		GetCapsuleComponent()->SetRelativeLocation(FVector::ZeroVector);
		GetCapsuleComponent()->SetRelativeRotation(FRotator::ZeroRotator);

		//Mesh
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
		GetMesh()->SetRelativeLocation((FVector::ZeroVector + FVector(0, 0, -90)));
		GetMesh()->SetRelativeRotation((FRotator::ZeroRotator + FRotator(0, 270, 0)));

		//CameraBool
		CameraBoom->SetRelativeLocation(FVector::ZeroVector);
		CameraBoom->SetRelativeRotation(FRotator::ZeroRotator);

		//Camera
		FollowCamera->SetRelativeLocation(FVector::ZeroVector);
		FollowCamera->SetRelativeRotation(FRotator::ZeroRotator);

		//Movement
		GetCharacterMovement()->StopMovementImmediately();

		// Re Enable Control
		EnableInput(GetLocalViewingPlayerController());
		bFallCameraActive = false;
		bIsRespawning = false;
		GetWorldTimerManager().ClearTimer(RespawnTimer);

		// Move player to checkpoint
		SetActorLocation(MyGameInstance->RespawnLocation, false, nullptr, ETeleportType::TeleportPhysics);
		SetActorRotation(FRotator::ZeroRotator); // Also reset rotation so they face the right way
		UE_LOG(LogTemp, Display, TEXT("Respawn on checkpoint location : %s"), *MyGameInstance->RespawnLocation.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Checkpoint just restart game"));
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
	}
}

void AUE5_GameplaySystemsCharacter::DeathWidgetAnimation()
{
	if (DeathRef)
	{
		DeathWidget = CreateWidget<UDeath>(GetWorld(), DeathRef);
		if (DeathWidget)
		{
			DeathWidget->AddToViewport();
			DeathWidget->PlayDeathFadeOutAnim();
		}
		GetWorldTimerManager().ClearTimer(DeathWidgetTimer);
	}
}

void AUE5_GameplaySystemsCharacter::AddScore(int Score)
{
	Gold += Score;
	ScoreWidget->SetScore(Gold);
	MyGameInstance->SetScore(Gold);

	/// Health 
	HealthUI->InitializeHearts();
}

bool AUE5_GameplaySystemsCharacter::EnableTouchScreenMovement(UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AUE5_GameplaySystemsCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AUE5_GameplaySystemsCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AUE5_GameplaySystemsCharacter::TouchUpdate);
		UE_LOG(LogTemp, Display, TEXT("Touch Update in Enable Touch screen"));

		return true;
	}

	return false;
}

void AUE5_GameplaySystemsCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Start Touch Location
	StartTouchLoc = FVector2D(Location.X, Location.Y);
	UE_LOG(LogTemp, Display, TEXT("StartTouchLoc : %s"), *StartTouchLoc.ToString());

}

void AUE5_GameplaySystemsCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	UE_LOG(LogTemp, Display, TEXT("StartTouchLoc : %s"), *StartTouchLoc.ToString());

	// Current touch Location
	FVector2D CurrentTouchLoc = FVector2D(Location.X, Location.Y);
	UE_LOG(LogTemp, Display, TEXT("CurrentTouchLoc : %s"), *CurrentTouchLoc.ToString());

	// Update TouchLocation
	FVector2D MoveTouchDelta = CurrentTouchLoc - StartTouchLoc;
	UE_LOG(LogTemp, Display, TEXT("MoveTouchDelta : %s"), *MoveTouchDelta.ToString());

	// Apply In the Movement 
	AddControllerYawInput(MoveTouchDelta.X * TouchSensitivity);
	AddControllerPitchInput(MoveTouchDelta.Y * TouchSensitivity);

	UE_LOG(LogTemp, Display, TEXT("Location : %s"), *Location.ToString());

	// After this subtraction we set the current location to be the new start location for the next update.
	StartTouchLoc = CurrentTouchLoc;
	UE_LOG(LogTemp, Display, TEXT("StartTouchLoc_1 : %s"), *StartTouchLoc.ToString());

}

void AUE5_GameplaySystemsCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StartTouchLoc = FVector2D::ZeroVector;
	UE_LOG(LogTemp, Display, TEXT("End_TouchLoc : %s"), *StartTouchLoc.ToString());

}