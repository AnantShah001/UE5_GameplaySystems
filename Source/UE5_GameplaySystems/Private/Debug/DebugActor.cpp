// Anant Shah All Rights Reserved
// Used for learning Actor lifecycle and runtime debugging

#include "Debug/DebugActor.h"
#include "Debug/DebugInfoComponent.h"


// Sets default values
ADebugActor::ADebugActor()
{
	UE_LOG(LogTemp, Log, TEXT("DebugActor Constructor"));
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DebugInfoComponent = CreateDefaultSubobject<UDebugInfoComponent>(TEXT("DebugInfoComponent"));
}

// Called when the game starts or when spawned
void ADebugActor::BeginPlay()
{
	Super::BeginPlay();
	// Use UE_Log to print a debug message to the output log
	UE_LOG(LogTemp, Display, TEXT("DebugActor : BeginPlay"));

	// Use this to print a debug message on the game screen for 10 seconds only.
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, TEXT("DebugActor : Hello Unreal Engine"));
	}*/

}

// Called every frame
void ADebugActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

