// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TriangleNode.h"
#include "LevelGeneration.generated.h"

class AIcosphereGridActor;
class UTriangleNode;

USTRUCT()
struct FLevelGenerationSettings {
	GENERATED_BODY()

public:

	UPROPERTY()
	int MaxRegionSize = 50;

	UPROPERTY()
	int MinRegionSize = 10;
};

USTRUCT()
struct FLevelRegion {
	GENERATED_BODY()

public:
	UPROPERTY()
	TSet<UTriangleNode*> Region;

	int Size() { return Region.Num(); }
	ETileType GetTileType() 
	{ 
		if (Region.Num() == 0) return ETileType::TT_Grass;
		return Region.Array()[0]->GetTileType(); 
	}

	bool operator==(const FLevelRegion& other) const
	{
		return (Region.Difference(other.Region).Num() == 0 &&
			other.Region.Difference(Region).Num() == 0);
	}

	bool operator!=(const FLevelRegion& other) const
	{
		return (Region.Difference(other.Region).Num() != 0 ||
			other.Region.Difference(Region).Num() != 0);
	}
};


/**
 * 
 */
UCLASS()
class ICOSPHEREGRID_API ULevelGeneration : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	static void GenerateLevel(AIcosphereGridActor* Grid, FLevelGenerationSettings& Settings);

	UFUNCTION()
	static TArray<FLevelRegion> SortIntoRegionsByType(FLevelRegion DivideRegion, FLevelGenerationSettings& Settings);

	UFUNCTION()
	static FLevelRegion RemoveLargestRegion(TArray<FLevelRegion>& Regions, FLevelGenerationSettings& Settings);

	UFUNCTION()
	static void MergeSmallRegionsIntoNeighbours(TArray<FLevelRegion>& Regions, FLevelGenerationSettings& Settings);

	UFUNCTION()
	static TArray<FLevelRegion> InsertSubRegion(FLevelRegion Region, FLevelGenerationSettings& Settings);

	UFUNCTION()
	static void MergeWaterAndHoles(AIcosphereGridActor* Grid, FLevelGenerationSettings& Settings);
};
