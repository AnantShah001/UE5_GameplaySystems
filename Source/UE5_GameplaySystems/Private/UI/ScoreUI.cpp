 // Anant Shah All Rights Reserved


#include "UI/ScoreUI.h"
#include "Components/TextBlock.h"

void UScoreUI::SetScore(int Score)
{
	if (GoldScore)
	{
		GoldScore->SetText(FText::AsNumber(Score));
	}
}