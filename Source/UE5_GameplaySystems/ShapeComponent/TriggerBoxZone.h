// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TriggerBoxZone.generated.h"

/** UCLASS macro makes this visible to the Unreal Engine.
 *  ClassGroup = (Custom): Categories it in the "Add Component" menu.
 *  meta = (BlueprintSpawnableComponent): Allows you to add this component to any Blueprint Actor.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) //Show in UE_Editor
class UE5_GAMEPLAYSYSTEMS_API UTriggerBoxZone : public UBoxComponent
{
	GENERATED_BODY() // Generates internal engine code required for Unreal (boilerplate).

public:
	// Construction
	UTriggerBoxZone();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(EditAnyWhere)
	FText Message;

	virtual void BeginPlay() override ;
    // Functions are protected so they can be overridden by child classes if needed
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
