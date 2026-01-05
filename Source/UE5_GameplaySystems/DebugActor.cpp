// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugActor.h"

// Sets default values
ADebugActor::ADebugActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADebugActor::BeginPlay()
{
	Super::BeginPlay();
	// Use UE_Log to print a debug message to the output log
	UE_LOG(LogTemp, Display, TEXT("DebugActor : BeginPlay"))

	// Use this to print a debug message on the game screen for 10 seconds only.
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, TEXT("DebugActor : Hello Unreal Engine"));
	}
	
}

// Called every frame
void ADebugActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

