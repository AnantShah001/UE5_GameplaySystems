// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class UE5_GAMEPLAYSYSTEMS_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// StaticMeshComponent
	UPROPERTY(VisibleDefaultsOnly, Category = "Item")
	UStaticMeshComponent* Mesh;

	// Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category = "Item")
	USphereComponent* CollisionComp;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	float CollisionRadius = 10.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
