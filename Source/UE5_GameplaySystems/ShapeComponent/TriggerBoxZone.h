// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TriggerBoxZone.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) //Show in UE_Editor
class UE5_GAMEPLAYSYSTEMS_API UTriggerBoxZone : public UBoxComponent
{
	GENERATED_BODY()

public:
	// Construction
	UTriggerBoxZone();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override ;

};
