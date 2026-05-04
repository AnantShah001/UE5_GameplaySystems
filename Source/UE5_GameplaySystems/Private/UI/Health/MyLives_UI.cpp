// Anant Shah All Rights Reserved

#include "UI/Health/MyLives_UI.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UMyLives_UI::PlayHeartLoseAnim()
{
	if (HeartLoseAnim)
	{
		PlayAnimation(HeartLoseAnim);
		UE_LOG(LogTemp, Error, TEXT("HeartLoseAnim Playing Animation"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HeartLoseAnim Not Play"));
	}
}

void UMyLives_UI::SetNumber(int Num)
{
	if (HeartNumber)
	{
		HeartNumber->SetText(FText::AsNumber(Num));
	}
}
