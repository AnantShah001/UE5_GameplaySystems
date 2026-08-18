// Anant Shah All Rights Reserved


#include "UI/Android_ScreenControls_UI/CursorJoystickWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"


FReply UCursorJoystickWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	UE_LOG(LogTemp, Error, TEXT("StartTouch"));
	bIsCursorActive = true;

	// Capture the local coordinates of the initial touch relative to this widget
	JoystickCenterPosition = InGeometry.AbsoluteToLocal(InPointerEvent.GetScreenSpacePosition());

	return FReply::Handled().CaptureMouse(TakeWidget());
}

FReply UCursorJoystickWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	UE_LOG(LogTemp, Error, TEXT("MoveTouch"));

	FVector2D CurrentTouchPos = InGeometry.AbsoluteToLocal(InPointerEvent.GetScreenSpacePosition());
	
	// Calculate direction and distance from Touch to the center point
	FVector2D CursorOffset = CurrentTouchPos - JoystickCenterPosition;
	float Distance = CursorOffset.Size();

	UE_LOG(LogTemp, Warning, TEXT(" Current Thumb: %s |-| Offset : %s |=| Distance : %f"),
		*CurrentTouchPos.ToString(), *CursorOffset.ToString(), Distance);

	// Clamp the touch position within your maximum radius boundary
	if (Distance > MaxMovementRadius)
	{
		CursorOffset = CursorOffset.GetSafeNormal() * MaxMovementRadius;
	}

	// Update the visual position of the thumb widget
	UpdateJoystickVisuals(CursorOffset);

	// Normalize data to a clean 0.0 -> 1.0 scale for the framework
	FVector2D CursorNormalized = CursorOffset / MaxMovementRadius;
	float CursorDistance = CursorNormalized.Size();

	AssignTask(CursorNormalized);

	return FReply::Handled();
}

FReply UCursorJoystickWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	UE_LOG(LogTemp, Error, TEXT("EndTouch"));

	// Reset everything cursor back to center when the player lifts their thumb
	ClampedInputVector = FVector2D::ZeroVector;
	UpdateJoystickVisuals(FVector2D::ZeroVector);

	bIsCursorActive = false;

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

void UCursorJoystickWidget::AssignTask(FVector2D CursorNormalized)
{
	UE_LOG(LogTemp, Display, TEXT("AssignTask()"));
}
