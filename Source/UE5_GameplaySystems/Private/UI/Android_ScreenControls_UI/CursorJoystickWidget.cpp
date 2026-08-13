// Anant Shah All Rights Reserved


#include "UI/Android_ScreenControls_UI/CursorJoystickWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"


FReply UCursorJoystickWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	UE_LOG(LogTemp, Error, TEXT("StartTouch"));

	// Capture the local coordinates of the initial touch relative to this widget
	JoystickCenterPosition = InGeometry.AbsoluteToLocal(InPointerEvent.GetScreenSpacePosition());

	//return FReply::Handled().CaptureMouse(this);
	return FReply::Handled().CaptureMouse(TakeWidget());//(TSharedFromThis);

}

FReply UCursorJoystickWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	UE_LOG(LogTemp, Error, TEXT("MoveTouch"));

	FVector2D CurrentTouchPos = InGeometry.AbsoluteToLocal(InPointerEvent.GetScreenSpacePosition());
	
	// Calculate direction and distance from the center point
	FVector2D Offset = CurrentTouchPos - JoystickCenterPosition;
	float Distance = Offset.Size();

	UE_LOG(LogTemp, Warning, TEXT("1) Current Thumb: %s | Offset : %s"), *CurrentTouchPos.ToString(), *Offset.ToString());

	UE_LOG(LogTemp, Warning, TEXT("2) Distance: %f"), Distance);

	// Clamp the touch position within your maximum radius boundary
	if (Distance > MaxMovementRadius)
	{
		Offset = Offset.GetSafeNormal() * MaxMovementRadius;
		UE_LOG(LogTemp, Warning, TEXT("3) Offset (MaxMovement Radius): %s"), *Offset.ToString());

	}

	// 1. Update the visual position of the thumb widget
	UpdateJoystickVisuals(Offset);

	UE_LOG(LogTemp, Warning, TEXT("4) Selected-Offset : %s"), *Offset.ToString());


	// 2. Normalize raw data to a clean 0.0 -> 1.0 scale for the framework
	FVector2D RawNormalized = Offset / MaxMovementRadius;
	float InputMagnitude = RawNormalized.Size();

	UE_LOG(LogTemp, Warning, TEXT("5) RawNormalized : %s | Input Magnitude: %f"), *RawNormalized.ToString(), InputMagnitude);

	// 3. Apply your custom gating logic for your Blend Space
	if (InputMagnitude < WalkThreshold)
	{
		UE_LOG(LogTemp, Warning, TEXT("6) InputMagnitude < 0.3f - Death Zone"));
		ClampedInputVector = FVector2D::ZeroVector; // Inside dead-zone
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("7) InputMagnitude > 0.3f - Jog"));
		ClampedInputVector = RawNormalized.GetSafeNormal() * 0.3f; // Force clean walk value
		
		UE_LOG(LogTemp, Warning, TEXT("8) Clamped Input Vector (* 0.3f) : %s "), *ClampedInputVector.ToString());

		ClampedInputVector = RawNormalized.GetSafeNormal();

		UE_LOG(LogTemp, Warning, TEXT("9) Clamped Input Vector (Normal) : %s "), *ClampedInputVector.ToString());

	}

	return FReply::Handled();
}

FReply UCursorJoystickWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	UE_LOG(LogTemp, Error, TEXT("EndTouch"));

	// Reset everything back to center when the player lifts their thumb
	ClampedInputVector = FVector2D::ZeroVector;
	UpdateJoystickVisuals(FVector2D::ZeroVector);

	return FReply::Handled().ReleaseMouseCapture();
}

void UCursorJoystickWidget::UpdateJoystickVisuals(FVector2D ThumbPosition)
{
	if (JoystickThumb)
	{
		UCanvasPanelSlot* ThumbSlot = Cast<UCanvasPanelSlot>(JoystickThumb->Slot);
		if (ThumbSlot)
		{
			// Move the thumb slot by the calculated math offset
			ThumbSlot->SetPosition(ThumbPosition);
		}
	}
}
