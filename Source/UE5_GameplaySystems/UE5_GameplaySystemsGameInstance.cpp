// Anant Shah All Rights Reserved


#include "UE5_GameplaySystemsGameInstance.h"




UUE5_GameplaySystemsGameInstance::UUE5_GameplaySystemsGameInstance()
{
	//
}

void UUE5_GameplaySystemsGameInstance::RespawnPlayer(FVector Location)
{
	RespawnLocation = Location;
	UE_LOG(LogTemp, Warning, TEXT("In Game Instance store Checkpoint Location"));
}

void UUE5_GameplaySystemsGameInstance::SetScore(int Score)
{
	Gold = Score;
	UE_LOG(LogTemp, Warning, TEXT("In Game Instance store Gold Score"));
}