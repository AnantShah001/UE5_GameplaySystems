// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugActor.generated.h"

class UDebugInfoComponent;

UCLASS()
class UE5_GAMEPLAYSYSTEMS_API ADebugActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebugActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere)
	class UDebugInfoComponent* DebugInfoComponent;
};
