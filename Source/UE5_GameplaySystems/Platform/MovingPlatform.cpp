// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "DrawDebugHelpers.h"

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
	// Debug
	DrawDebugSphere(GetWorld(), StartLocation, 20.f, 12, FColor::Cyan, true, -1.f);
	FVector EndLocation = StartLocation + PlatformVelocity.GetSafeNormal() * MoveDistance;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, true, -1.f);
	DrawDebugSphere(GetWorld(), EndLocation, 20.f, 12, FColor::Red, true, -1.f);
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	DrawDebugPoint(GetWorld(), GetActorLocation(), 20, FColor::Blue, false, -1.0f,0);
	if(ShouldRotate) RotatePlatform(DeltaTime);

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

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	AddActorLocalRotation(RotationVelocity);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100, FColor::Red, false, -1.f);
	DrawDebugPoint(GetWorld(), GetActorLocation() + GetActorForwardVector() * 100, 20, FColor::Red, false, -1.0f, 0);
}
