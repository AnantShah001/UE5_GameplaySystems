// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TimerManager.h"
#include "UE5_GameplaySystemsCharacter.generated.h"

//Reduce compile time
//Avoid circular dependencies
//These are pointers or references, so forward declaration is enough
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)// Allows reading config values from DefaultGame.ini
class AUE5_GameplaySystemsCharacter : public ACharacter
{
	GENERATED_BODY()// Required for UE's reflection system

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera that follows the character at the end of the boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	// Input Mapping Context that defines which input actions are active for this character.
	// Added to the local player at runtime using Enhanced Input subsystem.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	// Represents the jump input for this character.
	// Triggered when the player presses or releases the jump key or button.
	// Bound to the Jump() and StopJumping() functions in SetupPlayerInputComponent().
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DebugAction;

	/** Move Input Action */
	// Represents the movement input for this character.
	// Typically returns a Vector2D where X = right / left, Y = forward / back.
	// Bound to the Move() function to move the character relative to camera direction.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	// Represents the camera rotation input for this character.
	// Typically returns a Vector2D where X = yaw(left / right), Y = pitch(up / down).
	// Bound to the Look() function to rotate the camera / controller.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	AUE5_GameplaySystemsCharacter();

	virtual void Tick(float DeltaTime) override;

	void HandleDeath();

protected:

	/** Called for movement input */
	// Handles movement input (X = right/left, Y = forward/back).
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	// Handles look input (X = yaw, Y = pitch).
	void Look(const FInputActionValue& Value);

	void DebugActionPressed();
			
	FTimerHandle RestartTimer;

	void RestartLevel();

	bool bFallCameraActive = false;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// called when the game starts or when spawned; used to add input mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom (spring arm) component **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns the follow camera component **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

