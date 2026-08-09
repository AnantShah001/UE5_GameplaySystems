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
#include "Data/Struct/ControlSpeed.h"
#include "UE5_GameplayPlayerController.h"



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
GetCharacterMovement()->MaxWalkSpeed = 390.f;
GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

// Create camera boom (spring arm) to position camera behind the character
CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
CameraBoom->SetupAttachment(RootComponent);
CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

// Create Cloth Components in Mesh (SkeletalMesh)
Helmet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Helmet"));
Helmet->SetupAttachment(GetMesh());

Chest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chest"));
Chest->SetupAttachment(GetMesh());

Pants = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pants"));
Pants->SetupAttachment(GetMesh());

Shoes = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes"));
Shoes->SetupAttachment(GetMesh());

// Create a follow camera
FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AUE5_GameplaySystemsCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetClothLeaderPose();
}

// Called when the game starts or when the character is spawned
void AUE5_GameplaySystemsCharacter::BeginPlay()
{
	// Call the base class BeginPlay (important for inherited functionality)
	Super::BeginPlay();

	CurrentLifeLine = MaxLifeLine;

	MyGameInstance = GetGameInstance<UUE5_GameplaySystemsGameInstance>();
	if (!MyGameInstance)return;
	MyGameInstance->RespawnPlayer(GetActorLocation());
	MyGameInstance->HealthLifeLine(CurrentLifeLine);

	//Add Input Mapping Context
	// Get the PlayerController controlling this character
	MyController = Cast<AUE5_GameplayPlayerController>(Controller);
	if (MyController)
	{
		// Get the Enhanced Input subsystem for the local player
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyController->GetLocalPlayer()))
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
		// Create the Health UI Widget and add it to the viewport.
		HealthUI = CreateWidget<UHealth_UI>(GetWorld(), Health_UI_Ref);
		if (HealthUI)
		{
			HealthUI->AddToViewport();
			for (int i = 0; i < CurrentLifeLine; i++)
			{
				HealthUI->InitializeHearts();
			}
		}

		// Free Look Curve TimeLine SetUp
		if (FreeLookCurve)
		{
			// Bind the timeline progress event to a function
			FOnTimelineFloat FreeLookProgress;
			FreeLookProgress.BindUFunction(this, FName("FreeLookTimelineProgress"));
			FreeLookTimeLine.AddInterpFloat(FreeLookCurve, FreeLookProgress);

			// Bind the timeline finished event to a function
			FOnTimelineEvent FreeLookFinished;
			FreeLookFinished.BindUFunction(this, FName("FreeLookTimelineFinished"));
			FreeLookTimeLine.SetTimelineFinishedFunc(FreeLookFinished);
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
	// Update the FreeLook timeline if it's playing
	if (FreeLookTimeLine.IsPlaying()) FreeLookTimeLine.TickTimeline(DeltaTime);
	
	// Update the character rotation based on FreeLook Mode;
	//if (bIsFreeLook == false) SetActorRotation(FRotator(0.f, RotationValue().Yaw, 0.f));
	UpdateControlledRotations();
}

//////////////////////////////////////////////////////////////////////////
// Input
// Binds Enhanced Input actions to character movement functions.
// This is called once when the Pawn is possessed by a Controller.
void AUE5_GameplaySystemsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		// Moving
		// Bind movement input (WASD / joystick) to the Move() function
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUE5_GameplaySystemsCharacter::Move);

		// Jumping
		// Bind jump input: start jumping when key/button is pressed
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AUE5_GameplaySystemsCharacter::Jump);
		// Bind jump input: stop jumping when key/button is released
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AUE5_GameplaySystemsCharacter::StopJumping);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AUE5_GameplaySystemsCharacter::Runing);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AUE5_GameplaySystemsCharacter::Runing);

		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AUE5_GameplaySystemsCharacter::Walking);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Completed, this, &AUE5_GameplaySystemsCharacter::Walking);

		// Looking
		// Bind look input (mouse / right joystick) to the Look() function
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUE5_GameplaySystemsCharacter::Look);

		EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Triggered, this, &AUE5_GameplaySystemsCharacter::DebugActionPressed);
		
		//Free Look 
		EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Started, this, &AUE5_GameplaySystemsCharacter::FreeLook_Start);
		EnhancedInputComponent->BindAction(FreeLookAction, ETriggerEvent::Completed, this, &AUE5_GameplaySystemsCharacter::FreeLook_Release);

		// Mobile Touch
		EnableTouchScreenMovement(EnhancedInputComponent);

	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
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
	CurrentLifeLine -= 1;
	MyGameInstance->HealthLifeLine(CurrentLifeLine);

	bIsRespawning = true;
	GetMesh()->SetSimulatePhysics(true);

	DisableInput(nullptr);
	bFallCameraActive = true;
	GetWorldTimerManager().SetTimer(DeathWidgetTimer, this, &AUE5_GameplaySystemsCharacter::DeathWidgetAnimation, 2.0f, false);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AUE5_GameplaySystemsCharacter::RespawnPlayer, 3.0f, false);

	// HealthLifeLine
	MyGameInstance->HeartLifeLine -= 1;
	HealthUI->PlayRemoveHeartAnim();
	
	if (bIsFreeLook) FreeLook_Release(); // When player dies and is in FreeLook mode, we need to release the FreeLook mode to avoid camera issues.
	if (DeathSound) UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

}

void AUE5_GameplaySystemsCharacter::RespawnPlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("Respawn Player"));
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);

	//HealthUI->RemoveHeart();
	if(CurrentLifeLine >= 1)
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
		// SetClothLeaderPose();
		Helmet->ForceClothNextUpdateTeleportAndReset();
        Chest->ForceClothNextUpdateTeleportAndReset();
        Pants->ForceClothNextUpdateTeleportAndReset();
        Shoes->ForceClothNextUpdateTeleportAndReset();
		UE_LOG(LogTemp, Display, TEXT("Respawn on checkpoint location"));
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
	//UE_LOG(LogTemp, Display, TEXT("StartTouchLoc : %s"), *StartTouchLoc.ToString());

}

void AUE5_GameplaySystemsCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	//UE_LOG(LogTemp, Display, TEXT("StartTouchLoc : %s"), *StartTouchLoc.ToString());

	// Current touch Location
	FVector2D CurrentTouchLoc = FVector2D(Location.X, Location.Y);
	//UE_LOG(LogTemp, Display, TEXT("CurrentTouchLoc : %s"), *CurrentTouchLoc.ToString());

	// Update TouchLocation
	FVector2D MoveTouchDelta = CurrentTouchLoc - StartTouchLoc;
	//UE_LOG(LogTemp, Display, TEXT("MoveTouchDelta : %s"), *MoveTouchDelta.ToString());

	// Apply In the Movement 
	AddControllerYawInput(MoveTouchDelta.X * TouchSensitivity);
	AddControllerPitchInput(MoveTouchDelta.Y * TouchSensitivity);

	//UE_LOG(LogTemp, Display, TEXT("Location : %s"), *Location.ToString());

	// After this subtraction we set the current location to be the new start location for the next update.
	StartTouchLoc = CurrentTouchLoc;
	//UE_LOG(LogTemp, Display, TEXT("StartTouchLoc_1 : %s"), *StartTouchLoc.ToString());

}

void AUE5_GameplaySystemsCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StartTouchLoc = FVector2D::ZeroVector;
	//UE_LOG(LogTemp, Display, TEXT("End_TouchLoc : %s"), *StartTouchLoc.ToString());

}

void AUE5_GameplaySystemsCharacter::SetClothLeaderPose()
{
	Shoes->SetLeaderPoseComponent(GetMesh(), true, false);
	Pants->SetLeaderPoseComponent(GetMesh(), true, false);
	Chest->SetLeaderPoseComponent(GetMesh(), true, false);
	Helmet->SetLeaderPoseComponent(GetMesh(), true, false);
}

// Free Look Mode
void AUE5_GameplaySystemsCharacter::FreeLook_Start()
{
	//bIsFreeLook =  Value.Get<bool>();

	bIsFreeLook = true;
	UE_LOG(LogTemp, Warning, TEXT("1) IsFreeLook : Start"));

	//FreeLookStart = MyCharacter->GetControlRotation();
	FreeLookStart = GetCameraBoom()->GetTargetRotation().Quaternion();

	UE_LOG(LogTemp, Warning, TEXT("2) FreeLook_Start: %s "), *FreeLookStart.ToString());
}

void AUE5_GameplaySystemsCharacter::FreeLook_Release()
{
	if (!FreeLookCurve) return;
	//FreeLookEnd = MyCharacter->GetActorRotation();
	FreeLookEnd = GetCameraBoom()->GetTargetRotation().Quaternion();
	UE_LOG(LogTemp, Warning, TEXT("3) FreeLook : Release"));
	FreeLookTimeLine.PlayFromStart();
}

void AUE5_GameplaySystemsCharacter::FreeLookTimelineProgress(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("FreeLook_Value: %f | Oter Time is : %f "), Value, FreeLookTimeLine.GetPlaybackPosition());
	UE_LOG(LogTemp, Warning, TEXT("4) FreeLook : Progress"));
	FQuat LerpFreeLookQuat = FQuat::Slerp(FreeLookEnd, FreeLookStart, Value); //FreeLookTimeLine.GetPlaybackPosition()
	UE_LOG(LogTemp, Warning, TEXT("5) SetRotation :%s"), *LerpFreeLookQuat.Rotator().ToString());

	if(MyController) MyController->SetControlRotation(LerpFreeLookQuat.Rotator());
}

void AUE5_GameplaySystemsCharacter::FreeLookTimelineFinished()
{
	bIsFreeLook = false;
	UE_LOG(LogTemp, Warning, TEXT("6) FreeLook : Finished"));
}

FRotator AUE5_GameplaySystemsCharacter::RotationValue()
{
	FRotator Rotation;
	if (bIsFreeLook)
	{
		Rotation = FreeLookStart.Rotator();
	}
	else
	{
		Rotation = GetControlRotation();
	}
	return Rotation;
}

// Player Movement System
// Called when movement input is received (WASD / joystick)
// Moves the character forward/back and right/left based on the input vector
void AUE5_GameplaySystemsCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	// Get the input vector (X = right/left, Y = forward/back)
	MovementVector = Value.Get<FVector2D>();
	
	// Player Move Smoothly
	SmoothSpeed();

	// find out which way is forward
	const FRotator Rotation = RotationValue(); // GetControlRotation(); ?????
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

void AUE5_GameplaySystemsCharacter::SmoothSpeed()
{
	MovementSpeed();
	float CurrentSpeed = GetVelocity().Size() + InterpSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("Current Speed : %f"), CurrentSpeed);

	if (CurrentSpeed <= WalkSpeed)
	{
		GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AUE5_GameplaySystemsCharacter::MovementSpeed()
{
	FName DataRow = MovementPosition();
	const UDataTable* Data_Table = SpeedDataTable.DataTable;

	if (Data_Table != nullptr)
	{
		FControlSpeed* Data = Data_Table->FindRow<FControlSpeed>(DataRow, TEXT("Movement Data"));
		if (Data != nullptr)
		{
			InterpSpeed = Data->InterpSpeed;

			if (MovementVector.Y > 0.f)
			{
				WalkSpeed = Data->Forward_Dir;
			}
			else
			{
				WalkSpeed = Data->Other_Dir;
			}
		}
	}
}

FName AUE5_GameplaySystemsCharacter::MovementPosition()
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

	//UE_LOG(LogTemp, Display, TEXT("Position : %s"), *Position.ToString());

	// Return Movement Position for DataTable
	return Position;
}

/// <summary>
/// /
/// </summary>
//void AUE5_GameplaySystemsCharacter::Jump()
//{
//	Jump();
//}
//
//void AUE5_GameplaySystemsCharacter::StopJumping()
//{
//	StopJumping();
//}

void AUE5_GameplaySystemsCharacter::Runing(const FInputActionValue& Value)
{
	bIsRuning = Value.Get<bool>();

	if (Value.Get<bool>()) bIsWalking = false;
}

void AUE5_GameplaySystemsCharacter::Walking(const FInputActionValue& Value)
{
	bIsWalking = Value.Get<bool>();

	if (Value.Get<bool>()) bIsRuning = false;
}

void AUE5_GameplaySystemsCharacter::UpdateControlledRotations()
{
	if (!MyController) return;

	float Speed = GetVelocity().Size();
	if (Speed > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("1) Speed : True"));
		if (bIsFreeLook)
		{
			UE_LOG(LogTemp, Warning, TEXT("2) Free_Look : True"));
			GetCharacterMovement()->bOrientRotationToMovement = false;
			bUseControllerRotationYaw = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("3) Free_Look : False"));
			GetCharacterMovement()->bOrientRotationToMovement = false;
			bUseControllerRotationYaw = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("4) Speed : False"));
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}
}