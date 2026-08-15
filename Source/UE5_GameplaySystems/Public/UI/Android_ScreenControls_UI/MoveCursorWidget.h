// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Android_ScreenControls_UI/CursorJoystickWidget.h"
#include "MoveCursorWidget.generated.h"

class AUE5_GameplaySystemsCharacter;

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UMoveCursorWidget : public UCursorJoystickWidget
{
	GENERATED_BODY()


protected:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual void AssignTask(FVector2D CursorNormalized) override;

private:
	UPROPERTY()
	TObjectPtr<AUE5_GameplaySystemsCharacter> MyCharacter;
};
