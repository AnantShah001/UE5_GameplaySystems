// Anant Shah All Rights Reserved


#include "ShapeComponent/DeathTriggerBoxZone.h"
#include "Kismet/GameplayStatics.h"

UDeathTriggerBoxZone::UDeathTriggerBoxZone()
{
	UE_LOG(LogTemp, Warning, TEXT("DeathTriggerBoxZone"));
}

void UDeathTriggerBoxZone::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UDeathTriggerBoxZone::OnOverlapBegin);

}

void UDeathTriggerBoxZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy Player Name : %s"), *OtherActor->GetName());

}