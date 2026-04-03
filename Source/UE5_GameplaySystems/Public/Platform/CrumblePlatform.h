// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Platform/MovingPlatform.h"
#include "CrumblePlatform.generated.h"

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API ACrumblePlatform : public AMovingPlatform //AActor
{
	GENERATED_BODY()

public:
	ACrumblePlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnyWhere)
	class UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnyWhere)
	class UBoxComponent* StandBox;

	UPROPERTY(EditAnyWhere)
	class UGeometryCollectionComponent* CrumblePlatform; //UE5+ Version

	//UPROPERTY(VisibleAnywhere)
	//class UDestructibleComponent* Platform; //UE4 Version

	// Trigger Function
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Sequnce Function
	void CrumblingFall();
	void DestroyActor();

	FTimerHandle CrumbleTimerHandler;
	FTimerHandle DestroyActorTimerHandler;

	bool ShouldStand = true;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
