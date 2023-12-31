// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TriangleNode.generated.h"

class UTriangleLink;
class AIcosphereGridActor;

UENUM(BlueprintType)
enum class ETileType : uint8
{
	TT_Grass	UMETA(DisplayName="Grass"),
	TT_Mountain	UMETA(DisplayName="Mountain"),
	TT_Water	UMETA(DisplayName="Water"),
	TT_Hole		UMETA(DisplayName="Hole"),
	TT_MAX		UMETA(DisplayNam="MaxValue")
};

/**
 * 
 */
UCLASS()
class ICOSPHEREGRID_API UTriangleNode : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetVertices(AIcosphereGridActor* ParentGrid, const FVector& VertexOne, const FVector& VertexTwo, const FVector& VertexThree);

	UFUNCTION()
	void UpdateVertices(TArray<FVector> NewVertices);

	UFUNCTION()
	FVector GetCenterPosition() const;

	UFUNCTION()
	FVector GetUpDirection() const { return GetCenterPosition().GetSafeNormal(0.01f); }

	UFUNCTION()
	TArray<FVector> GetVertices() const { return TriangleVertices; }

	UFUNCTION()
	void AddNodeToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors);

	UFUNCTION()
	void AddLink(UTriangleLink* NewLink) { Links.Add(NewLink); }

	UFUNCTION()
	TArray<UTriangleLink*> GetLinks() { return Links; }

	UFUNCTION()
	ETileType GetTileType() const { return TileType; }

	UFUNCTION()
	void SetTileType(ETileType Type) { TileType = Type; }

	UFUNCTION()
	bool IsNeighbour(UTriangleNode* OtherNode);

	UFUNCTION()
	AIcosphereGridActor* GetGrid() const { return Grid; }

	UFUNCTION()
	static FLinearColor GetColorPerType(ETileType Type);

	UFUNCTION()
	AActor* GetUnit() { return Unit; }

	UFUNCTION()
	bool TrySetUnit(AActor* NewUnit);

	UFUNCTION()
	bool IsOccupied() { return IsValid(Unit); }

protected:
	UPROPERTY()
	TArray<FVector> TriangleVertices;

	UPROPERTY()
	TArray<UTriangleLink*> Links;
	
	UPROPERTY()
	ETileType TileType;

	UFUNCTION()
	void AddGrassToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors);
	
	UFUNCTION()
	void AddMountainToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors);
	
	UFUNCTION()
	void AddWaterToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors);
	
	UFUNCTION()
	void AddHoleToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors);

	UFUNCTION()
	void AddBorderToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors);

	//Gets vertices shared with another node, with consistent ordering.
	UFUNCTION()
	TArray<FVector> GetSharedVertices(UTriangleNode* OtherNode);

	UPROPERTY()
	TObjectPtr<AIcosphereGridActor> Grid;

	UPROPERTY()
	TObjectPtr<AActor> Unit;

	
};
