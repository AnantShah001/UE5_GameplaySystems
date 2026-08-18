// Anant Shah All Rights Reserved


#include "UI/Android_ScreenControls_UI/Android_ScreenControls_UI.h"
#include "UE5_GameplaySystems/UE5_GameplaySystemsCharacter.h"
#include "UE5_GameplaySystems/UE5_GameplayPlayerController.h"
#include "components/Button.h"

void UAndroid_ScreenControls_UI::NativeConstruct()
{
	Super::NativeConstruct();

	MyCharacter = Cast<AUE5_GameplaySystemsCharacter>(GetOwningPlayerPawn());

	MyController = Cast<AUE5_GameplayPlayerController>(GetOwningPlayer());

	if (JumpButton)
	{
		JumpButton->OnClicked.AddDynamic(this, &UAndroid_ScreenControls_UI::JumpButtonPressed);
	}

	if (PauseButton)
	{
		PauseButton->OnClicked.AddDynamic(this, &UAndroid_ScreenControls_UI::PauseButtonPressed);
	}
}

void UAndroid_ScreenControls_UI::JumpButtonPressed()
{
	if (MyCharacter)
	{
		MyCharacter->Jump();
		UE_LOG(LogTemp, Display, TEXT("Jump Button"));

	}
}

void UAndroid_ScreenControls_UI::PauseButtonPressed()
{
	if (MyController)
	{
		MyController->PauseMenuWidget();
		UE_LOG(LogTemp, Display, TEXT("Pause Button"));
	}
}

