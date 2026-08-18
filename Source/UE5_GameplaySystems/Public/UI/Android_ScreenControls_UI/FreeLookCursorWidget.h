// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Android_ScreenControls_UI/CursorJoystickWidget.h"
#include "FreeLookCursorWidget.generated.h"

class AUE5_GameplaySystemsCharacter;

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UFreeLookCursorWidget : public UCursorJoystickWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	//Native UMG Touch Events Overrides
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InPointerEvent) override;

	virtual void AssignTask(FVector2D CursorNormalized) override;

private:
	UPROPERTY()
	TObjectPtr<AUE5_GameplaySystemsCharacter> MyCharacter;
	
};
