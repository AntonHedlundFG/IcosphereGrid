// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IcosphereGridActor.generated.h"

class UProceduralMeshComponent;
class UTriangleNode;
class UTriangleLink;
class UMaterial;

UCLASS()
class ICOSPHEREGRID_API AIcosphereGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIcosphereGridActor();

	UFUNCTION()
	float GetRadius() const { return SphereRadius; }
	
	UFUNCTION()
	float GetMountainHeight() const { return MountainHeight; }
	
	UFUNCTION()
	float GetHoleDepth() const { return HoleDepth; }

	UFUNCTION()
	float GetWaterDepth() const { return WaterDepth; }

	UFUNCTION()
	int GetMaxRegionSize() const { return MaxRegionSize; }

	UFUNCTION()
	int GetMinRegionSize() const { return MinRegionSize; }

	UFUNCTION()
	TArray<UTriangleNode*> GetNodes() { return Nodes; }

	/** Finds first node that intersects with the described Ray
	* Returns nullptr if there is no intersection.
	*/
	UFUNCTION()
	UTriangleNode* GetNodeFromRaycast(FVector RayStart, FVector RayDirection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int32 Subdivisions = 3;

	UPROPERTY(EditAnywhere)
	float SphereRadius = 1000.0f;

	UPROPERTY(EditAnywhere)
	float MountainHeight = 0.05f;

	UPROPERTY(EditAnywhere)
	float HoleDepth = 0.15f;

	UPROPERTY(EditAnywhere)
	float WaterDepth = 0.025f;

	UPROPERTY(EditAnywhere)
	int MaxRegionSize = 25;

	UPROPERTY(EditAnywhere)
	int MinRegionSize = 4;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProceduralMeshComponent> ProceduralMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMaterial> Material;

	UPROPERTY()
	TArray<UTriangleNode*> Nodes;

	UPROPERTY()
	TArray<UTriangleLink*> Links;

	UFUNCTION()
	void GenerateIcosphereGrid(int32 SubdivisionLevels, float Radius);

	UFUNCTION()
	void GenerateMesh();

	//Divides each triangle into four in a tri-force pattern.
	UFUNCTION()
	void SubdivideIcosphereMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles);

	//Adds the point between the two points with index p1 and p2 into the list of vertices, and returns its index.
	//If there's already a vertex at that point, simply return its index instead.
	UFUNCTION()
	int32 AddMiddlePoint(TArray<FVector>& Vertices, int32 p1, int32 p2);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
