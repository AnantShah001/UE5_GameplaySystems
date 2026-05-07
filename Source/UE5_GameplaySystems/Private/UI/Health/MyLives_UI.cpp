// Anant Shah All Rights Reserved

#include "UI/Health/MyLives_UI.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UMyLives_UI::PlayHeartLoseAnim()
{
	if (HeartLoseAnim)
	{
		if(HeartLoseSound)PlaySound(HeartLoseSound);
		// Create Delegate
		FWidgetAnimationDynamicEvent EndDelegate;

		// Bind it to our Function
		EndDelegate.BindDynamic(this, &UMyLives_UI::OnHeartLoseAnimFinished);

		//HeartLoseAnim->BindToAnimationFinished(this, EndDelegate);//1)

		// Tell Animation to call this delegate when it finishes
		BindToAnimationFinished(HeartLoseAnim, EndDelegate); //2)

		PlayAnimation(HeartLoseAnim);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Play HeartLoseAnim"));
	}
}

void UMyLives_UI::OnHeartLoseAnimFinished()
{
	if (HeartLoseAnim)
	{
		// Instead of RemoveFromParent, we "Broadcast" (shout) that we are done
		OnAnimationFinishedDelegate.Broadcast(this);
	}
}

