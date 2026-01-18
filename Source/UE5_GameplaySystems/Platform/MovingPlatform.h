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

private:
	// Create Variable
	FVector StartLocation;

	UPROPERTY(EditAnywhere, Category = "Moving") // Variable Show in UE_Editer
	FVector PlatformVelocity = FVector(100, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Moving")
	float MoveDistance = 1000;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	FRotator RotationVelocity;

	UPROPERTY(EditAnyWhere, Category = "Rotation")
	bool ShouldRotate = false;

	// Create Function No Value Return Type
	void MovePlatform(float DeltaTime);

	void RotatePlatform(float DeltaTime);
};
