// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TriangleLink.h"

/**
 * 
 */
class ICOSPHEREGRID_API TriangleNode
{
public:

	TriangleNode(FVector VertexOne, FVector VertexTwo, FVector VertexThree);

	UFUNCTION()
	FVector GetCenterPosition();

	UFUNCTION()
	TArray<FVector> GetVertices() { return Vertices; }

	UFUNCTION()
	void AddNodeToMesh(TArray<FVector>& Vertices, TArray<FVector>& Triangles, TArray<FVector>& Normals, TArray<FColor>& Colors);

private:
	UPROPERTY()
	TArray<FVector> Vertices;

	UPROPERTY()
	TArray<TriangleLink> Links;
};
