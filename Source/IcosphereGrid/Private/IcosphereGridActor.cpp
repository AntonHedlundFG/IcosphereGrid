// Fill out your copyright notice in the Description page of Project Settings.


#include "IcosphereGridActor.h"
#include "ProceduralMeshComponent.h"
#include "TriangleNode.h"
#include "TriangleLink.h"
//TEMPORARY INCLUDE
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AIcosphereGridActor::AIcosphereGridActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	SetRootComponent(ProceduralMeshComponent);
}

// Called when the game starts or when spawned
void AIcosphereGridActor::BeginPlay()
{
	Super::BeginPlay();
	ProceduralMeshComponent->ClearAllMeshSections();
	GenerateIcosphereMesh(Subdivisions, SphereRadius);
}

// Called every frame
void AIcosphereGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIcosphereGridActor::GenerateIcosphereMesh(int32 SubdivisionLevels, float Radius)
{
	const float Phi = 1.618033f; //Golden Ratio

	//All vertices on an icosahedron
	const FVector Vertices[] = {
		FVector(-1.0f, 0, Phi), FVector(1.0f, 0, Phi), FVector(-1.0f, 0, -Phi), FVector(1.0f, 0, -Phi),
		FVector(0, Phi, 1.0f), FVector(0, Phi, -1.0f), FVector(0, -Phi, 1.0f), FVector(0, -Phi, -1.0f),
		FVector(Phi, 1.0f, 0), FVector(-Phi, 1.0f, 0), FVector(Phi, -1.0f, 0), FVector(-Phi, -1.0f, 0)
	};

	//Initial triangles of the icosahedron definition above
	const int32 Triangles[] = {
	0, 4, 1, 0, 9, 4, 9, 5, 4, 4, 5, 8, 4, 8, 1,
	8, 10, 1, 8, 3, 10, 5, 3, 8, 5, 2, 3, 2, 7, 3,
	7, 10, 3, 7, 6, 10, 7, 11, 6, 11, 0, 6, 0, 1, 6,
	1, 10, 6, 9, 0, 11, 9, 11, 2, 9, 2, 5, 7, 2, 11
	};

	//Move vertices and triangles into manageable arrays.
	TArray<FVector> MeshVertices;
	TArray<int32> MeshTriangles;
	MeshVertices.Append(Vertices, sizeof(Vertices) / sizeof(Vertices[0]));
	MeshTriangles.Append(Triangles, sizeof(Triangles) / sizeof(Triangles[0]));

	//Divide each triangle into 4 subtriangles
	for (int32 i = 0; i < SubdivisionLevels; ++i)
		SubdivideIcosphereMesh(MeshVertices, MeshTriangles);

	//Project all vertices onto sphere of desired radius
	for (FVector& Vertex : MeshVertices)
	{
		Vertex.Normalize();
		Vertex *= Radius;
	}


	//Create Nodes
	for (int i = 0; i < MeshTriangles.Num(); i += 3)
	{
		UTriangleNode* NewNode = NewObject<UTriangleNode>(this);
		NewNode->SetVertices(
			MeshVertices[MeshTriangles[i]], 
			MeshVertices[MeshTriangles[i + 1]], 
			MeshVertices[MeshTriangles[i + 2]]);
		Nodes.Add(NewNode);
	}

	//Create Links between neighbouring Nodes
	for (int i = 0; i < Nodes.Num(); i++)
	{
		for (int j = i + 1; j < Nodes.Num(); j++)
		{
			if (Nodes[i]->IsNeighbour(Nodes[j]))
			{
				UTriangleLink* NewLink;

				NewLink = NewObject<UTriangleLink>(this);
				NewLink->SetLinks(Nodes[i], Nodes[j]);
				Nodes[i]->AddLink(NewLink);
				Links.Add(NewLink);

				NewLink = NewObject<UTriangleLink>(this);
				NewLink->SetLinks(Nodes[j], Nodes[i]);
				Nodes[j]->AddLink(NewLink);
				Links.Add(NewLink);
			}
		}
	}

	TArray<FVector> NodeVertices;
	TArray<int32> NodeTriangles;
	TArray<FVector> NodeNormals;
	TArray<FColor> NodeColors;

	//Add mesh information for each node
	for (UTriangleNode* Node : Nodes)
	{
		Node->AddNodeToMesh(NodeVertices, NodeTriangles, NodeNormals, NodeColors);
	}

	ProceduralMeshComponent->CreateMeshSection(0, NodeVertices, NodeTriangles, NodeNormals, TArray<FVector2D>(), NodeColors, TArray<FProcMeshTangent>(), true);

}

void AIcosphereGridActor::SubdivideIcosphereMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
	TArray<int32> NewTriangles;

	for (int32 i = 0; i < Triangles.Num(); i += 3)
	{
		int32 v1 = Triangles[i];
		int32 v2 = Triangles[i + 1];
		int32 v3 = Triangles[i + 2];

		int32 m1 = AddMiddlePoint(Vertices, v1, v2);
		int32 m2 = AddMiddlePoint(Vertices, v2, v3);
		int32 m3 = AddMiddlePoint(Vertices, v3, v1);

		NewTriangles.Add(v1); NewTriangles.Add(m1); NewTriangles.Add(m3);
		NewTriangles.Add(v2); NewTriangles.Add(m2); NewTriangles.Add(m1);
		NewTriangles.Add(v3); NewTriangles.Add(m3); NewTriangles.Add(m2);
		NewTriangles.Add(m1); NewTriangles.Add(m2); NewTriangles.Add(m3);
	}

	Triangles = MoveTemp(NewTriangles);
}

int32 AIcosphereGridActor::AddMiddlePoint(TArray<FVector>& Vertices, int32 p1, int32 p2)
{
	FVector point1 = Vertices[p1];
	FVector point2 = Vertices[p2];
	FVector middle = (point1 + point2) * 0.5f;

	int32 Index = Vertices.Find(middle);

	if (Index == INDEX_NONE)
	{
		Vertices.Add(middle);
		return Vertices.Num() - 1;
	}
	return Index;

}