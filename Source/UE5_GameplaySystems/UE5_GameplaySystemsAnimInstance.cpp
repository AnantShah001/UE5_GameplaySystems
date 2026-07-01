// Anant Shah All Rights Reserved


#include "UE5_GameplaySystemsAnimInstance.h"
#include "UE5_GameplaySystemsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"


void UUE5_GameplaySystemsAnimInstance::NativeInitializeAnimation()
{
	MyCharacter = Cast<AUE5_GameplaySystemsCharacter>(TryGetPawnOwner());
	if (MyCharacter)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
	}
}

void UUE5_GameplaySystemsAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (MyCharacter)
	{
		//Speed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
		FVector Velocity = MyCharacter->GetVelocity();
		Speed = Velocity.Size();

		FRotator ControlRotation = MyCharacter->GetControlRotation();
		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, FRotator(0.f, ControlRotation.Yaw,0.f));
		
		FRotator ActorRotation = MyCharacter->GetActorRotation();
		FRotator DeltaRotator = (ControlRotation - ActorRotation).GetNormalized();

		Pitch = DeltaRotator.Pitch;
		Yaw = DeltaRotator.Yaw;

		UseControllerRotationYaw();
		InAirCheck();
	}
}

void UUE5_GameplaySystemsAnimInstance::UseControllerRotationYaw()
{
	//if (Speed > 0)
	//{
	//	MyCharacter->bUseControllerRotationYaw = true;
	//}
	//else if (Speed <= 0)
	//{
	//	MyCharacter->bUseControllerRotationYaw = false;
	//}
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
