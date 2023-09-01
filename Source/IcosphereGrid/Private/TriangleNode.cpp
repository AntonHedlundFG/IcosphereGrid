// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleNode.h"

TriangleNode::TriangleNode(FVector VertexOne, FVector VertexTwo, FVector VertexThree)
{
	Vertices.Add(VertexOne);
	Vertices.Add(VertexTwo);
	Vertices.Add(VertexThree);
}

FVector TriangleNode::GetCenterPosition()
{
	FVector Sum;
	for (FVector Vertex : Vertices)
	{
		Sum += Vertex;
	}
	return Sum / Vertices.Num();
}