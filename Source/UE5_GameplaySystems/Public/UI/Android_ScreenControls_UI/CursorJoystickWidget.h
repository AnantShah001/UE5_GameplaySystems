// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CursorJoystickWidget.generated.h"

class AUE5_GameplaySystemsCharacter;
class UImage;

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UCursorJoystickWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	// NativeTick allows us to send continuous input every frame
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> JoystickBackground;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> JoystickThumb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Joystick Settings")
	float MaxMovementRadius = 150.0f;

	// Input zone thresholds for Walk vs Jog/Run
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Joystick Settings")
	float WalkThreshold = 0.10f;

	// Stores the current clean -1.0 to 1.0 input vector for your character to read
	UPROPERTY(BlueprintReadOnly, Category = "Joystick Output")
	FVector2D ClampedInputVector;

	// Internal vector to track where the touch started
	FVector2D JoystickCenterPosition;

	//Native UMG Touch Events Overrides
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent) override;

	// Helper Function to Update the UI Thumb Visual Pointer 
	void UpdateJoystickVisuals(FVector2D ThumbPosition);

private:
	UPROPERTY()
	TObjectPtr<AUE5_GameplaySystemsCharacter> MyCharacter;

	bool bIsCursorActive = false;
};
