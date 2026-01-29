// Anant Shah All Rights Reserved


#include "Platform/MovingPlatform.h"
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
	StartRotation = GetActorRotation();

	StartRotationQuat = StartRotation.Quaternion();
	TargetRotationQuat = TargetRotation.Quaternion();

	// If we are returning, our "Target" is the TransferLocation
	if (!ShouldReturn)
	{
		// Calculate velocity toward TransferLocation using TransferSpeed
		FVector Direction = (TransferLocation - StartLocation).GetSafeNormal();
		PlatformVelocity = Direction * TransferSpeed;
		UE_LOG(LogTemp, Error, TEXT("Platform Velocity : %s"), *PlatformVelocity.ToString());

		// Update MoveDistance to be exactly the distance to the Transfer point
		MoveDistance = FVector::Dist(StartLocation, TransferLocation);

	}

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

	if (ShouldMove) MovePlatform(DeltaTime); // Should Move Check
	DrawDebugPoint(GetWorld(), GetActorLocation(), 20, FColor::Blue, false, -1.0f, 0);
	if (ShouldRotate) RotatePlatform(DeltaTime);// if ShouldRotate variable true. Then function cal every frame

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
		if (ShouldReturn)
		{
			// Normal Return Logic
			FVector MoveDirection = PlatformVelocity.GetSafeNormal();// Get Safe Normalized Direction Vector
			StartLocation = StartLocation + MoveDirection * MoveDistance;
			SetActorLocation(StartLocation);
			PlatformVelocity = -PlatformVelocity;//Direction Reverse(Velocity Reverse) 
			//UE_LOG(LogTemp, Display, TEXT("Move, Distance : %f | Location : %s"), DistanceMoved, *CurrentLocation.ToString());
		}
		else
		{
			// ShouldReturn is False Logic
			// Move to final position and stop moving forever
			SetActorLocation(StartLocation + PlatformVelocity.GetSafeNormal() * MoveDistance);
			ShouldMove = false;
		}
	}

}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	if (ShouldTargetRotation)
	{
		Alpha = FMath::Clamp(Alpha + (TargetRotationSpeed * DeltaTime), 0.0f, 1.0f);// clamp use for to between min & max value
		
		FQuat RotatorOffset = FQuat::SlerpFullPath(StartRotationQuat, TargetRotationQuat, Alpha);//for smooth interpolation

		SetActorRotation(RotatorOffset);
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100, FColor::Red, false, -1.f);
		DrawDebugPoint(GetWorld(), GetActorLocation() + GetActorForwardVector() * 100, 20, FColor::Red, false, -1.0f, 0);
		
		if (Alpha >= 1.0f)
		{
			ShouldRotate = false;
			FQuat S_Rotate = TargetRotationQuat;
			FQuat T_Rotate = StartRotationQuat;
			StartRotationQuat = S_Rotate;
			TargetRotationQuat = T_Rotate;
			Alpha = 0.0f;
		}
	}
	else
	{
		AddActorLocalRotation(RotationVelocity);
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100, FColor::Red, false, -1.f);
		DrawDebugPoint(GetWorld(), GetActorLocation() + GetActorForwardVector() * 100, 20, FColor::Red, false, -1.0f, 0);
	}

}


void AMovingPlatform::SetMovePlatform(bool bMove)
{
	ShouldMove = bMove;

}

void AMovingPlatform::SetRotatePlatform(bool bRotate)
{
	ShouldRotate = bRotate;
}
