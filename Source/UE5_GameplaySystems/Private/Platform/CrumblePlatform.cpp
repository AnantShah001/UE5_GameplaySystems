// Anant Shah All Rights Reserved


#include "Platform/CrumblePlatform.h"
#include "Components/BoxComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

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

	// Initialize Particle System
	TriggerParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TriggerParticle"));
	TriggerParticle->SetupAttachment(StandBox);
	TriggerParticle->bAutoActivate = false; 

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
		CameraShake();
		UnBind();
	}
}

void ACrumblePlatform::CrumblingFall()
{
	UE_LOG(LogTemp, Error, TEXT("StartCrumbling"));
	StandBox->DestroyComponent();
	ShouldMove = false;
	CrumblePlatform->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(DestroyActorTimerHandler, this, &ACrumblePlatform::DestroyActor, (CrumbleDelay + 5.0f), false);
}

void ACrumblePlatform::DestroyActor()
{
	Destroy();
}

void ACrumblePlatform::UnBind()
{
	TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &ACrumblePlatform::OnOverlapBegin);
	TriggerParticle->Activate(true);

	// Start the first delay (e.g., 2 seconds before parts drop)
	GetWorldTimerManager().SetTimer(CrumbleTimerHandler, this, &ACrumblePlatform::CrumblingFall, CrumbleDelay, false);

	if (CrumbleSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CrumbleSound, GetActorLocation());
	}
}

void ACrumblePlatform::CameraShake()
{
	if (CrumbleShakeCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("Camera Shake 1"));
		//UGameplayStatics::PlayWorldCameraShake(this, CrumbleShakeCamera, GetActorLocation(), 0.0f, 1000.0f,1.0f);

		APlayerController* MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (MyController)
		{
			UE_LOG(LogTemp, Warning, TEXT("Camera Shake 2"));
			//MyController->ClientPlayCameraShake(CrumbleShakeCamera);
			MyController->ClientStartCameraShake(CrumbleShakeCamera);
		}
	}
}