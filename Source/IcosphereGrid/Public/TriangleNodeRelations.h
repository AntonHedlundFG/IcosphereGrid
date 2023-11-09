// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TriangleNodeRelations.generated.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ETriangleNodeRelations : uint8
{
	TNR_NULL = 0				UMETA(Hidden),
	TNR_NORTH = 1 << 0			UMETA(DisplayName = "North"),
	TNR_SOUTH = 1 << 1			UMETA(DisplayName = "South"),
	TNR_WEST = 1 << 2			UMETA(DisplayName = "West"),
	TNR_EAST = 1 << 3			UMETA(DisplayName = "East"),
	TNR_SAMEREGION = 1 << 4		UMETA(DisplayName = "Same Region"),
	TNR_NEIGHBOUR = 1 << 5		UMETA(DisplayName = "Neighbour"),
	TNR_HASFREEPATH = 1 << 6	UMETA(DisplayName = "Has Free Path"),
	TNR_MAX = 1 << 7			UMETA(Hidden)
};
ENUM_CLASS_FLAGS(ETriangleNodeRelations);

class UTriangleNode;
class AIcosphereGridActor;

/**
 * 
 */
UCLASS()
class ICOSPHEREGRID_API UTriangleNodeRelations : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable) 
	static bool IsNorthOf(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode);
	
	UFUNCTION(BlueprintCallable)
	static bool IsSouthOf(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode);
	
	UFUNCTION(BlueprintCallable)
	static bool IsWestOf(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode);
	
	UFUNCTION(BlueprintCallable)
	static bool IsEastOf(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode);
	
	//A region is a connected area of tiles of the same type.
	UFUNCTION(BlueprintCallable)
	static bool IsInSameRegion(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode);
	
	UFUNCTION(BlueprintCallable)
	static bool IsNeighbour(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode);
	
	UFUNCTION(BlueprintCallable)
	static bool HasFreePath(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode, TArray<ETileType> TravelableTileTypes);

	//Returns a ETriangleNodeRelations bitmask. If TravelableTileTypes is not specified
	//TNR_HASFREEPATH flag will always be 0.
	UFUNCTION(BlueprintCallable, meta = (AutoCreateRefTerm = "TravelableTileTypes"))
	static uint8 GetNodeRelations(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode, TArray<ETileType> TravelableTileTypes);
};
