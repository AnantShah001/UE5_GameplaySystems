// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UE5_GameplaySystemsAnimInstance.generated.h"

class AUE5_GameplaySystemsCharacter;
class UCharacterMovementComponent;
class AUE5_GameplayPlayerController;

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

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AUE5_GameplayPlayerController> MyController;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float CalculatDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float IfGreaterThen = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float IfLessThen = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float Target = 55.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float First_Target = 85.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float Second_Target = 50.f;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float Yaw;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsInAir = false;

private:

	void InAirCheck();

};
