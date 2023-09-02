// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LevelGeneration.generated.h"

USTRUCT()
struct FLevelGenerationSettings {
	GENERATED_BODY()
};

class AIcosphereGridActor;

/**
 * 
 */
UCLASS()
class ICOSPHEREGRID_API ULevelGeneration : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	static void GenerateLevel(AIcosphereGridActor* Grid, FLevelGenerationSettings Settings);

};
