// Anant Shah All Rights Reserved


#include "UI/Android_ScreenControls_UI/MoveCursorWidget.h"
#include "UE5_GameplaySystems/UE5_GameplaySystemsCharacter.h"
#include "InputActionValue.h"


void UMoveCursorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MyCharacter = Cast<AUE5_GameplaySystemsCharacter>(GetOwningPlayerPawn());
}

void UMoveCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsCursorActive)
	{
		//MyCharacter->Move(FInputActionValue(ClampedInputVector));
		MyCharacter->Move(FInputActionValue(FVector2D(ClampedInputVector.X, -ClampedInputVector.Y)));
	}
}

void UMoveCursorWidget::AssignTask(FVector2D CursorNormalized)
{
	float CursorDistance = CursorNormalized.Size();

	// 3. Apply your custom gating logic for your Blend Space
	if (CursorDistance < 0.10f)
	{
		MyCharacter->Runing(FInputActionValue(false));
		MyCharacter->Walking(FInputActionValue(false));
		UE_LOG(LogTemp, Warning, TEXT("6) CursorDistance < 0.1f - Idle State"));
	}

	else if (CursorDistance > 0.95f)
	{
		MyCharacter->Runing(FInputActionValue(true));
		MyCharacter->Walking(FInputActionValue(false));
		UE_LOG(LogTemp, Warning, TEXT("7) CursorDistance > 0.95f - Run State"));
	}

	else if (CursorDistance > 0.50f)
	{
		MyCharacter->Runing(FInputActionValue(false));
		MyCharacter->Walking(FInputActionValue(false));
		UE_LOG(LogTemp, Warning, TEXT("8) CursorDistance > 0.50f - Jog State"));
	}

	else
	{
		MyCharacter->Runing(FInputActionValue(false));
		MyCharacter->Walking(FInputActionValue(true));
		UE_LOG(LogTemp, Warning, TEXT("9) CursorDistance > 0.10f - Walk State"));
	}

	ClampedInputVector = CursorNormalized.GetSafeNormal();
	UE_LOG(LogTemp, Warning, TEXT("11) CursorDistance : %f | ClampedInputVector(Normal) : %s "), CursorDistance, *ClampedInputVector.ToString());
}
