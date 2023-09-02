// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TriangleNode.generated.h"

class UTriangleLink;


USTRUCT()
struct FMovementRestrictions
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsWater;

	UPROPERTY()
	bool bIsMountain;

	UPROPERTY()
	bool bIsHole;
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
	void SetVertices(const FVector& VertexOne, const FVector& VertexTwo, const FVector& VertexThree);

	UFUNCTION()
	FVector GetCenterPosition() const;

	UFUNCTION()
	FVector GetUpDirection() const { return GetCenterPosition().GetSafeNormal(0.01f); }

	UFUNCTION()
	TArray<FVector> GetVertices() const { return TriangleVertices; }

	UFUNCTION()
	void AddNodeToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FColor>& Colors) const;

	UFUNCTION()
	void AddLink(UTriangleLink* NewLink) { Links.Add(NewLink); }

	UPROPERTY()
	FMovementRestrictions MovementRestrictions;

	UFUNCTION()
	bool IsNeighbour(UTriangleNode* OtherNode);

protected:
	UPROPERTY()
	TArray<FVector> TriangleVertices;

	UPROPERTY()
	TArray<UTriangleLink*> Links;

};
