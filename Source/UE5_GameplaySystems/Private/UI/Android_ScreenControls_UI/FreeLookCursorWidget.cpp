// Anant Shah All Rights Reserved


#include "UI/Android_ScreenControls_UI/FreeLookCursorWidget.h"
#include "UE5_GameplaySystems/UE5_GameplaySystemsCharacter.h"
#include "InputActionValue.h"


void UFreeLookCursorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MyCharacter = Cast<AUE5_GameplaySystemsCharacter>(GetOwningPlayerPawn());
}

FReply UFreeLookCursorWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InPointerEvent);

	UE_LOG(LogTemp, Error, TEXT("StartTouch"));
	bIsCursorActive = true;

	// Capture the local coordinates of the initial touch relative to this widget
	JoystickCenterPosition = InGeometry.AbsoluteToLocal(InPointerEvent.GetScreenSpacePosition());

	MyCharacter->FreeLook_Start();

	return FReply::Handled().CaptureMouse(TakeWidget());
}

FReply UFreeLookCursorWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InPointerEvent);

	UE_LOG(LogTemp, Error, TEXT("EndTouch"));

	MyCharacter->FreeLook_Release();

	// Reset everything cursor back to center when the player lifts their thumb
	ClampedInputVector = FVector2D::ZeroVector;
	UpdateJoystickVisuals(FVector2D::ZeroVector);

	bIsCursorActive = false;

	return FReply::Handled().ReleaseMouseCapture();
}

void UFreeLookCursorWidget::AssignTask(FVector2D CursorNormalized)
{
	Super::AssignTask(CursorNormalized);

	float CursorDistance = CursorNormalized.Size();

	ClampedInputVector = CursorNormalized.GetSafeNormal();

	MyCharacter->AddControllerYawInput(CursorNormalized.X);
	MyCharacter->AddControllerPitchInput(CursorNormalized.Y);
}
