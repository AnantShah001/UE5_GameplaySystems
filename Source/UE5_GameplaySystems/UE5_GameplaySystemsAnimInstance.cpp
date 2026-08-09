// Anant Shah All Rights Reserved


#include "UE5_GameplaySystemsAnimInstance.h"
#include "UE5_GameplaySystemsCharacter.h"
#include "UE5_GameplayPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"


void UUE5_GameplaySystemsAnimInstance::NativeInitializeAnimation()
{
	MyCharacter = Cast<AUE5_GameplaySystemsCharacter>(TryGetPawnOwner());
	if (MyCharacter)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
		MyController = Cast<AUE5_GameplayPlayerController>(MyCharacter->GetController());
	}
}

void UUE5_GameplaySystemsAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (MyCharacter)
	{
		//Speed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
		FVector Velocity = MyCharacter->GetVelocity();
		Speed = Velocity.Size();

		//FRotator ControlRotation = MyCharacter->GetControlRotation();
		FRotator ActorRotation = MyCharacter->GetActorRotation();
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, FRotator(0.f, ActorRotation.Yaw,0.f));
		
		//FRotator ActorRotation = MyCharacter->GetActorRotation();
		FRotator ControlRotation = MyCharacter->GetControlRotation();
		FRotator DeltaRotator = (ControlRotation - ActorRotation).GetNormalized();

		Pitch = DeltaRotator.Pitch;
		Yaw = DeltaRotator.Yaw;

		InAirCheck();
	}
}


void UUE5_GameplaySystemsAnimInstance::InAirCheck()
{
	if (MyCharacterMovement->IsFalling())
	{
		IsInAir = true;
	}
	else
	{
		IsInAir = false;
	}
}
