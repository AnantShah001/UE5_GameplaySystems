// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TimerManager.h"
#include "Components/TimelineComponent.h"
#include "UE5_GameplaySystemsCharacter.generated.h"

//Reduce compile time
//Avoid circular dependencies
//These are pointers or references, so forward declaration is enough
//Forward Declaration

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class USoundBase;
class UDeath;
class UScoreUI;
class UUE5_GameplaySystemsGameInstance;
class AUE5_GameplayPlayerController;
class UHealth_UI;
class UCurveFloat;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)// Allows reading config values from DefaultGame.ini
class AUE5_GameplaySystemsCharacter : public ACharacter
{
	GENERATED_BODY()// Required for UE's reflection system

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera that follows the character at the end of the boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	// Use Cloth Component for Helmet, Chest, Pants, and Shoes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cloth, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Helmet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cloth, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Chest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cloth, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Pants;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cloth, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Shoes;
	
	/** MappingContext */
	// Input Mapping Context that defines which input actions are active for this character.
	// Added to the local player at runtime using Enhanced Input subsystem.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DebugAction;

	/** Look Input Action */
	// Represents the camera rotation input for this character.
	// Typically returns a Vector2D where X = yaw(left / right), Y = pitch(up / down).
	// Bound to the Look() function to rotate the camera / controller.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

public:
	AUE5_GameplaySystemsCharacter();

	virtual void Tick(float DeltaTime) override;

	void HandleDeath();

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UDeath> DeathRef;

	UPROPERTY()
	TObjectPtr<UDeath> DeathWidget;

	UPROPERTY(EditAnyWhere, Category = "Sound")
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UScoreUI> ScoreRef;

	UPROPERTY()
	TObjectPtr<UScoreUI> ScoreWidget;

	UPROPERTY()
	TObjectPtr<UUE5_GameplaySystemsGameInstance> MyGameInstance;

	UPROPERTY()
	TObjectPtr<AUE5_GameplayPlayerController> MyController;

	void DeathWidgetAnimation();

	int Gold;

	void AddScore(int Score);

	FVector RespawnPlayerLocationX;

	/// Health
	UPROPERTY(EditAnywhere)
	TSubclassOf<UHealth_UI> Health_UI_Ref;

	UPROPERTY()
	TObjectPtr<UHealth_UI> HealthUI;

	UPROPERTY(EditAnyWhere, Category = "Health")
	int MaxLifeLine = 3;

	int CurrentLifeLine;

protected:

	/** Called for looking input */
	// Handles look input (X = yaw, Y = pitch).
	void Look(const FInputActionValue& Value);

	void DebugActionPressed();
			
	FTimerHandle RespawnTimer;

	FTimerHandle DeathWidgetTimer;

	void RespawnPlayer();

	bool bFallCameraActive = false;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// called when the game starts or when spawned; used to add input mapping context
	virtual void BeginPlay();

	// This is the EXACT C++ equivalent of the Blueprint Construction Script
	virtual void OnConstruction(const FTransform& Transform) override;

	void SetClothLeaderPose();

public:
	/** Returns CameraBoom (spring arm) component **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns the follow camera component **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	// This is for Mobile touch input
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

	bool EnableTouchScreenMovement(UInputComponent* PlayerInputComponent);

	FVector2D StartTouchLoc;

	UPROPERTY(EditAnywhere, Category = "Input")
	float TouchSensitivity = 0.3f;

public:
	bool bIsRespawning = false;

// Free Look Mode
public:
	void FreeLook_Start();

	void FreeLook_Release();

	bool bIsFreeLook = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FreeLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	TObjectPtr<UCurveFloat> FreeLookCurve;

	FTimeline FreeLookTimeLine;

	UFUNCTION()
	void FreeLookTimelineProgress(float Value);

	FRotator RotationValue();

	UFUNCTION()
	void FreeLookTimelineFinished();

	FQuat FreeLookStart;
	FQuat FreeLookEnd;
	
// Player Movement System
	public:

		void Runing(const FInputActionValue& Value);

		void Walking(const FInputActionValue& Value);

	protected:
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> JumpAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		TObjectPtr<UInputAction> MoveAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		TObjectPtr<UInputAction> RunAction;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
		TObjectPtr<UInputAction> WalkAction;

		/** Called for movement input */
		// Handles movement input (X = right/left, Y = forward/back).
		void Move(const FInputActionValue& Value);

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsRuning = false;

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		bool bIsWalking = false;

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
		float WalkSpeed = 200.f;

		FVector2D MovementVector;
		
	private:

		void SmoothSpeed();

		void MovementSpeed();

		FName MovementPosition();
		float InterpSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true", RowType = "ControlSpeed"))
		FDataTableRowHandle SpeedDataTable;
		
		void UpdateControlledRotations();

};

