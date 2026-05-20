// Anant Shah All Rights Reserved


#include "UI/Menu/PauseMenu_UI.h"
#include "components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UE5_GameplaySystems/UE5_GameplayPlayerController.h"

void UPauseMenu_UI::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Display, TEXT("NativeConstruct"))

	//PlayerController = GetOwningPlayer();

	PlayerController = Cast< AUE5_GameplayPlayerController>(GetOwningPlayer());
	if (!PlayerController) return;

	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UPauseMenu_UI::OnClickPlayButton);
	}

	if (ReStartButton)
	{
		ReStartButton->OnClicked.AddDynamic(this, &UPauseMenu_UI::OnClickReStartButton);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UPauseMenu_UI::OnClickQuitButton);
	}
}

void UPauseMenu_UI::OnClickPlayButton()
{
	if (PlayButton)
	{
		PlayerController->PauseMenuWidget();
		UE_LOG(LogTemp, Display, TEXT("Play_Button Clicked"));

	}
}

void UPauseMenu_UI::OnClickReStartButton()
{
	if (ReStartButton)
	{
		PlayerController->PauseMenuWidget();
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		UE_LOG(LogTemp, Display, TEXT("ReStart_Button Clicked"));
	}
}

void UPauseMenu_UI::OnClickQuitButton()
{
	if (QuitButton)
	{
		UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
		UE_LOG(LogTemp, Display, TEXT("Quit_Button Clicked"));
	}
}


