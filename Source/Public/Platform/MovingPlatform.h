// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class UE5_GAMEPLAYSYSTEMS_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Moving")
	bool ShouldMove = true; // If false, never moves

	UPROPERTY(EditAnywhere, Category = "Moving")
	bool ShouldReturn = true; // If false, stops at destination

	void ActivatePlatform(bool bActivate);

private:
	// Create Variable
	FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = "Moving", meta = (EditCondition = "ShouldMove")) // Variable Show in UE_Editer
	FVector PlatformVelocity = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Moving", meta = (EditCondition = "ShouldMove"))
	float MoveDistance = 1000;

	UPROPERTY(EditAnywhere, Category = "Moving", meta = (EditCondition = "!ShouldReturn"))
	FVector TransferLocation; // Location to stop at if ShouldReturn is false

	UPROPERTY(EditAnywhere, Category = "Moving", meta = (EditCondition = "!ShouldReturn"))
	float TransferSpeed = 300.0f; // Speed specifically for the Transfer path

	UPROPERTY(EditAnyWhere, Category = "Rotation")
	bool ShouldRotate = false;

	UPROPERTY(EditAnywhere, Category = "Rotation" , meta = (EditCondition = "ShouldRotate"))
	FRotator RotationVelocity;

	// Create Function No Value Return Type
	void MovePlatform(float DeltaTime);

	void RotatePlatform(float DeltaTime);
};
