// Anant Shah All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ControlSpeed.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UE5_GAMEPLAYSYSTEMS_API FControlSpeed : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Forward_Dir = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Other_Dir = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InterpSpeed = 0.0f;
};
