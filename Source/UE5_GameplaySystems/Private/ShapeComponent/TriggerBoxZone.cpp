#include "ShapeComponent/TriggerBoxZone.h"
#include "Platform/MovingPlatform.h"
#include "Kismet/GameplayStatics.h"

//Construction 
UTriggerBoxZone::UTriggerBoxZone()
{
	// Enables the TickComponent function to run every frame 
	PrimaryComponentTick.bCanEverTick = false;

	// Sets this box to act as a "Trigger" 
	SetCollisionProfileName(TEXT("Trigger"));
	// Essential: Ensures the component is capable of firing "Overlap" events
	SetGenerateOverlapEvents(true);


}

void UTriggerBoxZone::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UTriggerBoxZone::OnOverlapBegin);
	OnComponentEndOverlap.AddDynamic(this, &UTriggerBoxZone::OnOverlapEnd);

}


void UTriggerBoxZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (OnlyPlayerCanTrigger && !OtherActor->IsA(APawn::StaticClass()))
	{
		return;
	}
	else
	{
		if (ShowMessageOnly)
		{
			MessageOnly();
			DestroyComponent();

		}
		else
		{
			MessageOnly();
			OnlyOneTimeUse();
		}
	}
}

void UTriggerBoxZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	if (OnlyPlayerCanTrigger && !OtherActor->IsA(APawn::StaticClass()))
	{
		return;
	}
	else
	{
		if (!OnlyWhenTigger) return;
		SetPlatformActive(false);
	}
}

void UTriggerBoxZone::SetPlatformActive(bool bActive)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), PlatformTag, FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (AMovingPlatform* Platform = Cast<AMovingPlatform>(Actor))
		{
			if (ShouldMove)
			{
				UE_LOG(LogTemp, Warning, TEXT("Should Move"));
				Platform->SetMovePlatform(bActive);
			}
			if (ShouldRotate)
			{
				UE_LOG(LogTemp, Warning, TEXT("Should Rotate"));
				Platform->SetRotatePlatform(bActive);
			}
		}
	}
}

void UTriggerBoxZone::OnlyOneTimeUse()
{
	if (OneTimeUse)
	{
		SetPlatformActive(true);
		DestroyComponent();
	}
	else
	{
		SetPlatformActive(true);
	}
}

void UTriggerBoxZone::MessageOnly()
{
	if (!Message.IsEmpty())
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, (TEXT("Message %s"), *Message.ToString()));
	}
}
