// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleNode.h"
#include "TriangleLink.h"
#include "IcosphereGridActor.h"

void UTriangleNode::SetVertices(AIcosphereGridActor* ParentGrid, const FVector& VertexOne, const FVector& VertexTwo, const FVector& VertexThree)
{
	Grid = ParentGrid;
	TriangleVertices.Add(VertexOne);
	TriangleVertices.Add(VertexTwo);
	TriangleVertices.Add(VertexThree);
	TileType = ETileType::TT_Hole;
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

void UTriangleNode::AddNodeToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors)
{
	switch (TileType)
	{
	case ETileType::TT_Grass:
		AddGrassToMesh(Vertices, Triangles, Normals, Colors);
		break;
	case ETileType::TT_Mountain:
		AddMountainToMesh(Vertices, Triangles, Normals, Colors);
		break;
	case ETileType::TT_Water:
		AddWaterToMesh(Vertices, Triangles, Normals, Colors);
		break;
	case ETileType::TT_Hole:
		AddHoleToMesh(Vertices, Triangles, Normals, Colors);
		break;
	}
	AddBorderToMesh(Vertices, Triangles, Normals, Colors);
}

void UTriangleNode::AddGrassToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors)
{
	for (FVector vertex : TriangleVertices)
	{
		Triangles.Add(Vertices.Num());
		Vertices.Add(vertex);
		Normals.Add(vertex.GetSafeNormal(0.01f));
		Colors.Add(UTriangleNode::GetColorPerType(ETileType::TT_Grass));
	}
}

void UTriangleNode::AddMountainToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors)
{
	//The triangles are determined by a pyramid shape, where 0, 1, 2 are the original vertices
	//and 3 is the new peak in the middle.
	int32 NewTriangles[] = {
		0, 1, 3,
		1, 2, 3, 
		0, 3, 2
	};
	for (int i = 0; i < 9; i++)
	{
		NewTriangles[i] += Vertices.Num();
	}
	Triangles.Append(NewTriangles, sizeof(NewTriangles) / sizeof(NewTriangles[0]));
	
	//Add the actual vertices according to numbering above.
	
	FLinearColor MountainColor = UTriangleNode::GetColorPerType(ETileType::TT_Mountain);
	for (FVector vertex : TriangleVertices)
	{
		Vertices.Add(vertex);
		Normals.Add(vertex.GetSafeNormal(0.01f));
		Colors.Add(MountainColor);
	}
	FVector MidPoint = GetCenterPosition() + GetUpDirection() * Grid->GetRadius() * Grid->GetMountainHeight();
	Vertices.Add(MidPoint);
	Normals.Add(MidPoint.GetSafeNormal(0.01f));
	Colors.Add(FMath::Lerp(MountainColor, FLinearColor::Black, 0.75f));



}

void UTriangleNode::AddWaterToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors)
{
	FLinearColor WaterColor = UTriangleNode::GetColorPerType(ETileType::TT_Water);
	//Hole floor
	for (FVector vertex : TriangleVertices)
	{
		Triangles.Add(Vertices.Num());
		Vertices.Add(vertex * (1 - Grid->GetWaterDepth()));
		Normals.Add(vertex.GetSafeNormal(0.01f));
		Colors.Add(WaterColor);
	}

	//Hole walls
	for (UTriangleLink* Link : Links)
	{
		//No wall between water tiles
		if (Link->GetTarget()->GetTileType() == ETileType::TT_Water)
			continue;

		TArray<FVector> SharedVertices = GetSharedVertices(Link->GetTarget());
		TArray<FVector> LoweredVertices;
		for (FVector SharedVertex : SharedVertices)
		{
			FVector LoweredVertex = SharedVertex * (1 - Grid->GetWaterDepth());
			LoweredVertices.Add(LoweredVertex);
		}
		Triangles.Add(Vertices.Num());
		Triangles.Add(Vertices.Num() + 1);
		Triangles.Add(Vertices.Num() + 3);
		Triangles.Add(Vertices.Num());
		Triangles.Add(Vertices.Num() + 3);
		Triangles.Add(Vertices.Num() + 2);
		Vertices.Add(SharedVertices[0]);
		Vertices.Add(SharedVertices[1]);
		Vertices.Add(LoweredVertices[0]);
		Vertices.Add(LoweredVertices[1]);
		FLinearColor OtherColor = UTriangleNode::GetColorPerType(Link->GetTarget()->GetTileType());
		Colors.Add(OtherColor);
		Colors.Add(OtherColor);
		Colors.Add(OtherColor);
		Colors.Add(OtherColor);
		FVector Normal = FVector::CrossProduct(
			LoweredVertices[1] - SharedVertices[0],
			SharedVertices[1] - SharedVertices[0]
		);
		Normal.Normalize();
		Normals.Add(Normal);
		Normals.Add(Normal);
		Normals.Add(Normal);
		Normals.Add(Normal);
	}
}

void UTriangleNode::AddHoleToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors)
{
	FLinearColor HoleColor = UTriangleNode::GetColorPerType(ETileType::TT_Hole);
	//Hole floor
	for (FVector vertex : TriangleVertices)
	{
		Triangles.Add(Vertices.Num());
		Vertices.Add(vertex * (1 - Grid->GetHoleDepth()));
		Normals.Add(vertex.GetSafeNormal(0.01f));
		Colors.Add(HoleColor);
	}

	//Hole walls
	for (UTriangleLink* Link : Links)
	{
		//No wall between hole tiles
		if (Link->GetTarget()->GetTileType() == ETileType::TT_Hole)
			continue;

		TArray<FVector> SharedVertices = GetSharedVertices(Link->GetTarget());
		TArray<FVector> LoweredVertices;
		for (FVector SharedVertex : SharedVertices)
		{
			FVector LoweredVertex = SharedVertex * (1 - Grid->GetHoleDepth());
			LoweredVertices.Add(LoweredVertex);
		}
		Triangles.Add(Vertices.Num());
		Triangles.Add(Vertices.Num() + 1);
		Triangles.Add(Vertices.Num() + 3);
		Triangles.Add(Vertices.Num());
		Triangles.Add(Vertices.Num() + 3);
		Triangles.Add(Vertices.Num() + 2);
		Vertices.Add(SharedVertices[0]);
		Vertices.Add(SharedVertices[1]);
		Vertices.Add(LoweredVertices[0]);
		Vertices.Add(LoweredVertices[1]);
		FLinearColor OtherColor = UTriangleNode::GetColorPerType(Link->GetTarget()->GetTileType());
		Colors.Add(OtherColor);
		Colors.Add(OtherColor);
		Colors.Add(HoleColor);
		Colors.Add(HoleColor);
		FVector Normal = FVector::CrossProduct(
			LoweredVertices[1] - SharedVertices[0],
			SharedVertices[1] - SharedVertices[0]
		);
		Normal.Normalize();
		Normals.Add(Normal);
		Normals.Add(Normal);
		Normals.Add(Normal);
		Normals.Add(Normal);
	}

}

void UTriangleNode::AddBorderToMesh(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FLinearColor>& Colors)
{
	for(UTriangleLink* Link : Links)
	{
		//No borders for holes
		if (Link->GetTarget()->GetTileType() == ETileType::TT_Hole)
			continue;

		TArray<FVector> SharedVertices = GetSharedVertices(Link->GetTarget());

		const float BorderWidth = 1.0f;

		FVector Forward = Link->GetTarget()->GetCenterPosition() - GetCenterPosition();
		Forward.Normalize();
		FVector Right = SharedVertices[1] - SharedVertices[0];
		Right.Normalize();
		FVector Up = FVector::CrossProduct(Right, Forward);

		FVector UpperLeft = SharedVertices[0] + Forward * BorderWidth;
		FVector UpperRight = SharedVertices[1] + Forward * BorderWidth;
		FVector LowerLeft = SharedVertices[0] - Forward * BorderWidth;
		FVector LowerRight = SharedVertices[1] - Forward * BorderWidth;

		UpperLeft *= 1.001f;
		UpperRight *= 1.001f;
		LowerLeft *= 1.001f;
		LowerRight *= 1.001f;

		Triangles.Add(Vertices.Num());
		Triangles.Add(Vertices.Num() + 1);
		Triangles.Add(Vertices.Num() + 3);
		Triangles.Add(Vertices.Num());
		Triangles.Add(Vertices.Num() + 3);
		Triangles.Add(Vertices.Num() + 2);

		Vertices.Add(UpperLeft);
		Vertices.Add(UpperRight);
		Vertices.Add(LowerLeft);
		Vertices.Add(LowerRight);

		Colors.Add(FLinearColor::Black);
		Colors.Add(FLinearColor::Black);
		Colors.Add(FLinearColor::Black);
		Colors.Add(FLinearColor::Black);

		Normals.Add(Up);
		Normals.Add(Up);
		Normals.Add(Up);
		Normals.Add(Up);
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


TArray<FVector> UTriangleNode::GetSharedVertices(UTriangleNode* OtherNode) 
{
	//This can only be guaranteed to work on neighbouring triangles.
	if (TriangleVertices.Num() != 3 || OtherNode->TriangleVertices.Num() != 3)
		return TArray<FVector>();

	//A messy algorithm. It guarantees the direction of the shared vertices
	//match the direction of the instance on which this function is called.
	//Specifically, if vertex 0 and 2 are shared, they should be ordered 2,0.
	TArray<FVector> Results;
	int FirstIndex = -1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (TriangleVertices[i] == OtherNode->TriangleVertices[j])
			{
				if (FirstIndex == -1)
				{
					Results.Add(TriangleVertices[i]);
					FirstIndex = i;
				}
				else if (i == FirstIndex + 2)
				{
					Results.Insert(TriangleVertices[i], 0);
				}
				else {
					Results.Add(TriangleVertices[i]);
				}
			}
		}
	}

	return Results;
}

FLinearColor UTriangleNode::GetColorPerType(ETileType Type)
{
	switch (Type)
	{
	case ETileType::TT_Grass:
		return FLinearColor::Gray;
	case ETileType::TT_Hole:
		return FLinearColor::Black;
	case ETileType::TT_Mountain:
		return FLinearColor::Red;
	case ETileType::TT_Water:
		return FLinearColor::Blue;
	default:
		return FLinearColor::White;
	}
}