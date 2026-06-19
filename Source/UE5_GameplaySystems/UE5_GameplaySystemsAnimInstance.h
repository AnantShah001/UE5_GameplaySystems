// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UE5_GameplaySystemsAnimInstance.generated.h"

class AUE5_GameplaySystemsCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class UE5_GAMEPLAYSYSTEMS_API UUE5_GameplaySystemsAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AUE5_GameplaySystemsCharacter> MyCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TObjectPtr<UCharacterMovementComponent> MyCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Yaw;
};
