// Anant Shah All Rights Reserved


#include "ShapeComponent/DeathTriggerBoxZone.h"
#include "UE5_GameplaySystems/UE5_GameplaySystemsCharacter.h"

//#include "UE5_GameplaySystemsCharacter.h"

UDeathTriggerBoxZone::UDeathTriggerBoxZone()
{
	//...
}

void UDeathTriggerBoxZone::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UDeathTriggerBoxZone::OnOverlapBegin);

}

void UDeathTriggerBoxZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Death Trigger Player Name : %s | Component: %s"), *OtherActor->GetName(), *OtherComp->GetName());

	if (AUE5_GameplaySystemsCharacter* PlayerCharacter = Cast<AUE5_GameplaySystemsCharacter>(OtherActor))
	{
		if (!PlayerCharacter->bIsRespawning)PlayerCharacter->HandleDeath();
	}
}
