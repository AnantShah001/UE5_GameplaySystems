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

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AUE5_GameplaySystemsCharacter

AUE5_GameplaySystemsCharacter::AUE5_GameplaySystemsCharacter()
{
	// Set size for collision capsule
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

	// Create a camera boom (pulls in towards the player if there is a collision)
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