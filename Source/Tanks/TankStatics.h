// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TankStatics.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UTankStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Math")
	static float FindDeltaAngleDegrees(float A1, float A2);

	UFUNCTION(BlueprintPure, Category = "Math|Rotator", meta = (keywords = "rotation rotate"))
	static bool FindLookAtAngle(const FVector2D& Start, const FVector2D& Target, float& angle);
	
};
