// Anant Shah All Rights Reserved

#include "UE5_Gameplay_AndroidScreen_HUD.h"
#include "UI/Android_ScreenControls_UI/Android_ScreenControls_UI.h"

void AUE5_Gameplay_AndroidScreen_HUD::BeginPlay()
{
    Super::BeginPlay();

    if (!AndroidWidgetClass) return;
    AndroidControls = CreateWidget<UAndroid_ScreenControls_UI>(GetWorld(), AndroidWidgetClass);
    
    // Add the widget to the viewport
    if (AndroidControls) AndroidControls->AddToViewport();
}
