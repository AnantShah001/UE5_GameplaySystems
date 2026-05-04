// Anant Shah All Rights Reserved


#include "UI/Health/Health_UI.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "UI/Health/MyLives_UI.h"

void UHealth_UI::InitializeHearts()
{
	if (!HeartWidgetClass || !HeartsContainerBox) return;
	
	Heart = CreateWidget<UMyLives_UI>(GetWorld(), HeartWidgetClass);
	if (Heart)
	{
		HeartsContainerBox->AddChildToHorizontalBox(Heart);

		HeartWidgets.Add(Heart);
		Heart->SetNumber(HeartWidgets.Num());
	}
}

void UHealth_UI::PlayRemoveHeartAnim()
{
	if (HeartWidgets.Last())
	{
		HeartWidgets.Last()->PlayHeartLoseAnim();
	}
}

void UHealth_UI::RemoveHeart()
{
	if (HeartWidgets.Last())
	{	
		HeartsContainerBox->RemoveChild(HeartWidgets.Last());
		HeartWidgets.Last()->RemoveFromParent();
		UE_LOG(LogTemp, Warning, TEXT("Heart Removed"));
	}
	HeartWidgets.Pop();
}
