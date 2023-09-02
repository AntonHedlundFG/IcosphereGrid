// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleNode.h"

void UTriangleNode::SetVertices(const FVector& VertexOne, const FVector& VertexTwo, const FVector& VertexThree)
{
	TriangleVertices.Add(VertexOne);
	TriangleVertices.Add(VertexTwo);
	TriangleVertices.Add(VertexThree);
}

FVector UTriangleNode::GetCenterPosition() const
{
	if (TriangleVertices.Num() == 0)
		return FVector::Zero();

	FVector Sum;
	for (FVector Vertex : TriangleVertices)
	{
		Sum += Vertex;
	}
	return Sum / TriangleVertices.Num();
}

void UTriangleNode::AddNodeToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FColor>& Colors) const
{
	for (FVector vertex : TriangleVertices)
	{
		Triangles.Add(Vertices.Num());
		Vertices.Add(vertex);
		Normals.Add(vertex.GetSafeNormal(0.01f));
		Colors.Add(FColor::Blue);
	}

}

bool UTriangleNode::IsNeighbour(UTriangleNode* OtherNode)
{
	int SharedVertices = 0;
	for (FVector ThisVertex : GetVertices())
	{
		for (FVector OtherVertex : OtherNode->GetVertices())
		{
			if (ThisVertex == OtherVertex)
			{
				SharedVertices++;
				break;
			}
		}
	}

	return SharedVertices == 2;
}