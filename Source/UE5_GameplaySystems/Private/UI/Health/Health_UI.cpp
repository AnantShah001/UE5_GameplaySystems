// Anant Shah All Rights Reserved


#include "UI/Health/Health_UI.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "UI/Health/MyLives_UI.h"

void UHealth_UI::InitializeHearts()
{
	if (!HeartWidgetClass || !HeartsContainerBox) return;
	
	UMyLives_UI* Heart = CreateWidget<UMyLives_UI>(GetWorld(), HeartWidgetClass);
	if (Heart)
	{
		N += 1;
		HeartsContainerBox->AddChildToHorizontalBox(Heart);
		Heart->AddToViewport();
		Heart->SetNumber(N);

		HeartWidgets.Add(Heart);
	}
}
