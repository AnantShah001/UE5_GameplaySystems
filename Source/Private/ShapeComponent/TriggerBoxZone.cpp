// Anant Shah All Rights Reserved


<<<<<<<< HEAD:Source/UE5_GameplaySystems/Private/ShapeComponent/TriggerBoxZone.cpp
#include "ShapeComponent/TriggerBoxZone.h"
#include "Platform/MovingPlatform.h"
========
#include "TriggerBoxZone.h"
#include "../Platform/MovingPlatform.h"
>>>>>>>> 7d16cbbe82439b11aa03589a1b9d1ff3bc9d11f5:Source/Private/ShapeComponent/TriggerBoxZone.cpp
#include "Kismet/GameplayStatics.h"

//Construction
UTriggerBoxZone::UTriggerBoxZone()
{
	// Enables the TickComponent function to run every frame
	PrimaryComponentTick.bCanEverTick = false;
	UE_LOG(LogTemp, Error, TEXT("UTriggerBoxZone : Construction"));

	// Sets this box to act as a "Trigger"
	SetCollisionProfileName(TEXT("Trigger"));
	// Essential: Ensures the component is capable of firing "Overlap" events	
	SetGenerateOverlapEvents(true);


}

void UTriggerBoxZone::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("UTriggerBoxZone : BeginPlay"));

	OnComponentBeginOverlap.AddDynamic(this, &UTriggerBoxZone::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UTriggerBoxZone::OnOverlapEnd);

}

void UTriggerBoxZone::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//..
}

void UTriggerBoxZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	UE_LOG(LogTemp, Display, TEXT("Trigger Enter : %s"), *OtherActor->GetName());
<<<<<<<< HEAD:Source/UE5_GameplaySystems/Private/ShapeComponent/TriggerBoxZone.cpp
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Enter in TriggerBoxZone "));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, (TEXT("Message : %s "), *Message.ToString()));

========
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,TEXT("Enter in TriggerBoxZone "));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, (TEXT("Message : %s "),*Message.ToString()));
	
>>>>>>>> 7d16cbbe82439b11aa03589a1b9d1ff3bc9d11f5:Source/Private/ShapeComponent/TriggerBoxZone.cpp
	if (bOnlyPlayerCanTrigger && !OtherActor->IsA(APawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin_Overlape"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin_Overlape_True"));
		SetPlatformActive(true);
	}
}

void UTriggerBoxZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;
	UE_LOG(LogTemp, Display, TEXT("Trigger Exit : %s"), *OtherActor->GetName());
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Exit TriggerBoxZone "));
<<<<<<<< HEAD:Source/UE5_GameplaySystems/Private/ShapeComponent/TriggerBoxZone.cpp

========
	
>>>>>>>> 7d16cbbe82439b11aa03589a1b9d1ff3bc9d11f5:Source/Private/ShapeComponent/TriggerBoxZone.cpp
	if (bOnlyPlayerCanTrigger && !OtherActor->IsA(APawn::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin_End_Overlape"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin_Overlape_False"));
		SetPlatformActive(false);
	}
}

void UTriggerBoxZone::SetPlatformActive(bool bActive)
{
	UE_LOG(LogTemp, Warning, TEXT("SetPlatformActive"));

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), PlatformTag, FoundActors);
<<<<<<<< HEAD:Source/UE5_GameplaySystems/Private/ShapeComponent/TriggerBoxZone.cpp

========
	
>>>>>>>> 7d16cbbe82439b11aa03589a1b9d1ff3bc9d11f5:Source/Private/ShapeComponent/TriggerBoxZone.cpp
	for (AActor* Actor : FoundActors)
	{
		if (AMovingPlatform* Platform = Cast<AMovingPlatform>(Actor))
		{
			Platform->ActivatePlatform(bActive);
		}
	}
}

