// Anant Shah All Rights Reserved


#include "Debug/DebugInfoComponent.h"

// Sets default values for this component's properties
UDebugInfoComponent::UDebugInfoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDebugInfoComponent::BeginPlay()
{
	Super::BeginPlay();
	PrintOwnerInfo(); // call the function to print owner info.
	// ...

}


// Called every frame
void UDebugInfoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDebugInfoComponent::PrintOwnerInfo() const
{
	AActor* Owner = GetOwner();
	UE_LOG(LogTemp, Warning, TEXT("DebugInfoComponent attached to : % s"), *Owner->GetName());

	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(2, 10.f, FColor::Blue, (TEXT("Owner Actor : %s"),*Owner->GetName()));
	}*/

}

