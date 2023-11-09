// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TriangleNodeRelations.generated.h"

UENUM(BlueprintType, meta = (Bitflags))
enum class ETriangleNodeRelations
{
	TNR_NULL			UMETA(Hidden),
	TNR_NORTH			UMETA(DisplayName = "North"),
	TNR_SOUTH			UMETA(DisplayName = "South"),
	TNR_WEST			UMETA(DisplayName = "West"),
	TNR_EAST			UMETA(DisplayName = "East"),
	TNR_SAMEREGION		UMETA(DisplayName = "Same Region"),
	TNR_NEIGHBOUR		UMETA(DisplayName = "Neighbour"),
	TNR_HASFREEPATH		UMETA(DisplayName = "Has Free Path")
};

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
	static int32 GetNodeRelations(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode, TArray<ETileType> TravelableTileTypes);
};
