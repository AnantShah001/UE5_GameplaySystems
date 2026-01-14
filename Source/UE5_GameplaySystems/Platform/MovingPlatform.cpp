// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	// Get Current Location
	FVector CurrentLocation = GetActorLocation();//This is Statement not a Expression.
	// Add Vector to that Location
	CurrentLocation = CurrentLocation + (PlatformVelocity * DeltaTime);
	// Set Actor Location
	SetActorLocation(CurrentLocation);
	// Check how far our Platform has gone.
	float DistanceMoved = FVector::Dist(StartLocation, CurrentLocation);//FVector(::) Use this to go inside the class FVector. And Dist is a Function of that class.
	// Reverse Direction of motion if gone too far
	if (DistanceMoved > MoveDistance)
	{
		FVector MoveDirection = PlatformVelocity.GetSafeNormal();// Get Safe Normalized Direction Vector
		StartLocation = StartLocation + MoveDirection * MoveDistance;
		SetActorLocation(StartLocation);
		PlatformVelocity = -PlatformVelocity;//Direction Reverse(Velocity Reverse) 
		UE_LOG(LogTemp, Display, TEXT("Move, Distance : %f | Location : %s"), DistanceMoved, *CurrentLocation.ToString());
	}
}