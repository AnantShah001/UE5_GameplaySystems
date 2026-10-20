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
		//FRotator DeltaRotator = (ControlRotation - ActorRotation).GetNormalized();

		//Pitch = DeltaRotator.Pitch;
		//Yaw = DeltaRotator.Yaw;

		// Try to Fixing Curser Direction Movement Issue
		CalculatDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, FRotator(0.f, ActorRotation.Yaw,0.f));
		float Abs_Direction = FMath::Abs(CalculatDirection);

		//UE_LOG(LogTemp, Error, TEXT(" ##1) CalculatDirection : %f "), CalculatDirection);
		//UE_LOG(LogTemp, Error, TEXT(" ##2) ABS_Direction : %f "), Abs_Direction);


		if (Abs_Direction > IfGreaterThen && Abs_Direction < IfLessThen)
		{
			//UE_LOG(LogTemp, Error, TEXT(" ##3) --------"));
			//Direction = Target * FMath::Sign(CalculatDirection);
			//}
			//UE_LOG(LogTemp, Error, TEXT(" ##3) 50 to 90"));
			//Direction = Target * FMath::Sign(CalculatDirection);
			if (Abs_Direction > Target) // 70
			{
				//UE_LOG(LogTemp, Error, TEXT(" ##4) >65"));

				Direction = First_Target * FMath::Sign(CalculatDirection); //90
			}
			else
			{
				//UE_LOG(LogTemp, Error, TEXT(" ##5) <60"));
				//UE_LOG(LogTemp, Error, TEXT("-- ##5) <60"));

				Direction = Second_Target * FMath::Sign(CalculatDirection); //45
			}
		}
		else
		{
			Direction = CalculatDirection;
		}
		//Direction = CalculatDirection;
		
		//UE_LOG(LogTemp, Error, TEXT(" ##6) Direction : %f "), Direction);

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
