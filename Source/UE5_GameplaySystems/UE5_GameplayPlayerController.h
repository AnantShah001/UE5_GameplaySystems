// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/TimelineComponent.h"
#include "UE5_GameplayPlayerController.generated.h"

class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class UPauseMenu_UI;
class AUE5_GameplaySystemsCharacter;
class UCharacterMovementComponent;
class UCurveFloat;


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

	void Jump();

	void StopJumping();

	void Runing(const FInputActionValue& Value);

	void Walking(const FInputActionValue& Value);

	void FreeLook(const FInputActionValue& Value);

	//UFUNCTION(BlueprintCallable, Category = "Camera")
	//void StartFreeLook(FRotator TargetRotation);

	void FreeLook_Start();
	void FreeLook_Release();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	/** Called for movement input */
	// Handles movement input (X = right/left, Y = forward/back).
	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsRuning = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsWalking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float WalkSpeed = 200.f;

	FVector2D MovementVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> FreeLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Timeline")
	TObjectPtr<UCurveFloat> FreeLookCurve;

	FTimeline FreeLookTimeLine;

	UFUNCTION()
	void FreeLookTimelineProgress(float Value);

	UFUNCTION()
	void FreeLookTimelineFinished();

	bool bIsFreeLook = false;

	FQuat FreeLookStart;
	FQuat FreeLookEnd;

private:

	bool bIsMainMenueWidget = false;

	void SmoothSpeed();

	void MovementSpeed();

	FName MovementPosition();
	float InterpSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true", RowType = "ControlSpeed"))
	FDataTableRowHandle SpeedDataTable;
};
