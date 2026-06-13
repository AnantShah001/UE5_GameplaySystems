// Fill out your copyright notice in the Description page of Project Settings.


#include "UE5_GameplaySystemsAnimInstance.h"
#include "UE5_GameplaySystemsCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
		Speed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
	}
}
