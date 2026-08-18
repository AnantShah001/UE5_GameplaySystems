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
		MyCharacter->Move(FInputActionValue(FVector2D(ClampedInputVector.X, -ClampedInputVector.Y)));
	}
}

void UMoveCursorWidget::AssignTask(FVector2D CursorNormalized)
{
	float CursorDistance = CursorNormalized.Size();

	if (CursorDistance < 0.10f)
	{
		MyCharacter->Runing(FInputActionValue(false));
		MyCharacter->Walking(FInputActionValue(false));
	}

	else if (CursorDistance > 0.95f)
	{
		MyCharacter->Runing(FInputActionValue(true));
		MyCharacter->Walking(FInputActionValue(false));
	}

	else if (CursorDistance > 0.50f)
	{
		MyCharacter->Runing(FInputActionValue(false));
		MyCharacter->Walking(FInputActionValue(false));
	}

	else
	{
		MyCharacter->Runing(FInputActionValue(false));
		MyCharacter->Walking(FInputActionValue(true));
	}

	ClampedInputVector = CursorNormalized.GetSafeNormal();
	UE_LOG(LogTemp, Warning, TEXT("11) CursorDistance : %f | ClampedInputVector(Normal) : %s "), CursorDistance, *ClampedInputVector.ToString());
}
