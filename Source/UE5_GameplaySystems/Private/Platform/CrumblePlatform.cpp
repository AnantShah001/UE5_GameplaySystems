// Anant Shah All Rights Reserved


#include "Platform/CrumblePlatform.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ACrumblePlatform::ACrumblePlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	//Initialize Box Trigger
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	//RootComponent = TriggerBox;
	TriggerBox->SetupAttachment(RootComponent);

	StandBox = CreateDefaultSubobject<UBoxComponent>("StandBox");
	StandBox->SetupAttachment(TriggerBox);

	//Initialize Fractured Mesh
	CrumblePlatform = CreateDefaultSubobject<UGeometryCollectionComponent>("Platform");
	CrumblePlatform->SetupAttachment(TriggerBox);

}

void ACrumblePlatform::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACrumblePlatform::OnOverlapBegin);

}

void ACrumblePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACrumblePlatform::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if it's the player (optional check)
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("Trigger"));
		UnBind();
	}
}

void ACrumblePlatform::CrumblingFall()
{
	UE_LOG(LogTemp, Error, TEXT("StartCrumbling"));
	StandBox->DestroyComponent();
	CrumblePlatform->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(DestroyActorTimerHandler, this, &ACrumblePlatform::DestroyActor, 7.0f, false);
}

void ACrumblePlatform::DestroyActor()
{
	Destroy();
}

void ACrumblePlatform::UnBind()
{
	TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &ACrumblePlatform::OnOverlapBegin);

	// Start the first delay (e.g., 2 seconds before parts drop)
	GetWorldTimerManager().SetTimer(CrumbleTimerHandler, this, &ACrumblePlatform::CrumblingFall, 1.2f, false);
}