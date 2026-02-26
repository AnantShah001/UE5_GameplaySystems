// Anant Shah All Rights Reserved


#include "UI/Death.h"
#include "Kismet/GameplayStatics.h"

void UDeath::PlayDeathFadeOutAnim()
{
	if (DeathFadeOut)
	{
		PlayAnimation(DeathFadeOut, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
		UE_LOG(LogTemp, Error, TEXT("DeathFadeOut Valid"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DeathFadeOut Not Valid"));
	}
}
