// Anant Shah All Rights Reserved


#include "UI/Android_ScreenControls_UI/FreeLookCursorWidget.h"
#include "UE5_GameplaySystems/UE5_GameplaySystemsCharacter.h"
#include "InputActionValue.h"


void UFreeLookCursorWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MyCharacter = Cast<AUE5_GameplaySystemsCharacter>(GetOwningPlayerPawn());
}

void UFreeLookCursorWidget::AssignTask(FVector2D CursorNormalized)
{
	Super::AssignTask(CursorNormalized);

	float CursorDistance = CursorNormalized.Size();

	UE_LOG(LogTemp, Warning, TEXT("CursorDistance : %f"), CursorDistance);

	MyCharacter->AddControllerYawInput(CursorNormalized.X);
	MyCharacter->AddControllerPitchInput(-CursorNormalized.Y);
	
}
