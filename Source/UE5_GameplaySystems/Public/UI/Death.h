// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Death.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UDeath : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Transient , meta = (BindWidgetAnim))
	class UWidgetAnimation* DeathFadeOut;

	void PlayDeathFadeOutAnim();
};
