// Anant Shah All Rights Reserved


#include "UI/Death.h"
#include "Kismet/GameplayStatics.h"

void UDeath::PlayDeathFadeOutAnim()
{
	if (DeathFadeOut)
	{
		// Create Delegate
		FWidgetAnimationDynamicEvent EndDelegate;

		// Bind it to our Function
		EndDelegate.BindDynamic(this, &UDeath::OnDeathFadeOutAnimFinished);

		// Tell Animation to call this delegate when it finishes

		BindToAnimationFinished(DeathFadeOut, EndDelegate);

		PlayAnimation(DeathFadeOut, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		UE_LOG(LogTemp, Error, TEXT("DeathFadeOut Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DeathFadeOut Not Valid"));
	}
}

void UDeath::OnDeathFadeOutAnimFinished()
{
	// This removes the widget from the screen and memory safely
	RemoveFromParent();
	UE_LOG(LogTemp, Error, TEXT("20) DeathFadeOut Widgetwidget from the screen "));
}
