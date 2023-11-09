// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleNodeRelations.h"
#include "TriangleNode.h"
#include "TriangleLink.h"
#include "IcosphereGridActor.h"
#include "Kismet/KismetSystemLibrary.h"

bool UTriangleNodeRelations::IsNorthOf(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode)
{
	return TargetNode->GetCenterPosition().Z > OtherNode->GetCenterPosition().Z;
}

bool UTriangleNodeRelations::IsSouthOf(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode)
{
	return TargetNode->GetCenterPosition().Z < OtherNode->GetCenterPosition().Z;
}

bool UTriangleNodeRelations::IsWestOf(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode)
{
	//This is a tricky calculation, as west/east is somewhat relative. 
	//My approach is to use the cross product between the TargetNode's surface normal and the global up vector
	//to get a "west vector", and then see if the dot product of the west vector and the location delta between
	//the two nodes give a positive value. 
	//We're assuming that the globe's north pole is aimed in the global up direction.

	const FVector TargetNodeSurfaceNormal = TargetNode->GetUpDirection();
	const FVector GlobalUp = FVector::UpVector;
	const FVector WestForTargetNode = FVector::CrossProduct(TargetNodeSurfaceNormal, GlobalUp);
	
	const FVector NodeDelta = OtherNode->GetCenterPosition() - TargetNode->GetCenterPosition();
	const float DotProduct = NodeDelta.Dot(WestForTargetNode);

	//If the Dot Product is greater than 0, it means the angle between our West vector and location delta
	//is less than 90 degrees, which is what we're looking for.
	return DotProduct > 0;
}

bool UTriangleNodeRelations::IsEastOf(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode)
{
	//This is the exact, but inverted, calculation used in InWestOf. We simply do the cross product in the
	//opposite order.

	const FVector TargetNodeSurfaceNormal = TargetNode->GetUpDirection();
	const FVector GlobalUp = FVector::UpVector;
	const FVector WestForTargetNode = FVector::CrossProduct(GlobalUp, TargetNodeSurfaceNormal);

	const FVector NodeDelta = OtherNode->GetCenterPosition() - TargetNode->GetCenterPosition();
	const float DotProduct = NodeDelta.Dot(WestForTargetNode);

	return DotProduct > 0;
}

//A region is a connected area of tiles of the same type.
bool UTriangleNodeRelations::IsInSameRegion(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode)
{
	ETileType Type = TargetNode->GetTileType();

	//Sanity check, obviously two tiles with different types aren't in the same region.
	if (OtherNode->GetTileType() != Type) return false;

	//Flood fill
	TArray<UTriangleNode*> OpenNodes;
	TArray<UTriangleNode*> ClosedNodes;
	OpenNodes.Add(TargetNode);
	while (!OpenNodes.IsEmpty())
	{
		UTriangleNode* NextNode = OpenNodes.Pop();
		for (UTriangleLink* Link : NextNode->GetLinks())
		{
			if (Link->GetTarget()->GetTileType() != Type)
				continue;
			if (Link->GetTarget() == OtherNode)
				return true;
			if (!ClosedNodes.Contains(Link->GetTarget()))
				OpenNodes.AddUnique(Link->GetTarget());
		}
		ClosedNodes.Add(NextNode);
	}
	return false;
}

bool UTriangleNodeRelations::IsNeighbour(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode)
{
	return TargetNode->IsNeighbour(OtherNode);
}

bool UTriangleNodeRelations::HasFreePath(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode, TArray<ETileType> TravelableTileTypes)
{
	//Sanity check, if either node is not of a valid travel type, there is no path.
	if (!TravelableTileTypes.Contains(TargetNode->GetTileType()) || !TravelableTileTypes.Contains(OtherNode->GetTileType()))
		return false;

	//Flood fill
	TArray<UTriangleNode*> OpenNodes;
	TArray<UTriangleNode*> ClosedNodes;
	OpenNodes.Add(TargetNode);
	while (!OpenNodes.IsEmpty())
	{
		UTriangleNode* NextNode = OpenNodes.Pop();
		for (UTriangleLink* Link : NextNode->GetLinks())
		{
			if (!TravelableTileTypes.Contains(Link->GetTarget()->GetTileType()))
				continue;
			if (Link->GetTarget() == OtherNode)
				return true;
			if (!ClosedNodes.Contains(Link->GetTarget()))
				OpenNodes.AddUnique(Link->GetTarget());
		}
		ClosedNodes.Add(NextNode);
	}
	return false;
}

int32 UTriangleNodeRelations::GetNodeRelations(AIcosphereGridActor* Grid, UTriangleNode* TargetNode, UTriangleNode* OtherNode, TArray<ETileType> TravelableTileTypes)
{
	int32 Relations = 0;
	if (IsNorthOf(Grid, TargetNode, OtherNode))
	{
		UKismetSystemLibrary::PrintString(Grid, FString("North"));
		Relations |= 1 << (int32)(ETriangleNodeRelations::TNR_NORTH);
	}
	if (IsSouthOf(Grid, TargetNode, OtherNode))
	{
		UKismetSystemLibrary::PrintString(Grid, FString("South"));
		Relations |= 1 << (int32)(ETriangleNodeRelations::TNR_SOUTH);
	}
	if (IsWestOf(Grid, TargetNode, OtherNode))
	{
		UKismetSystemLibrary::PrintString(Grid, FString("West"));
		Relations |= 1 << (int32)(ETriangleNodeRelations::TNR_WEST);
	}
	if (IsEastOf(Grid, TargetNode, OtherNode))
	{
		UKismetSystemLibrary::PrintString(Grid, FString("East"));
		Relations |= 1 << (int32)(ETriangleNodeRelations::TNR_EAST);
	}
	if (IsInSameRegion(Grid, TargetNode, OtherNode))
	{
		UKismetSystemLibrary::PrintString(Grid, FString("SameRegion"));
		Relations |= 1 << (int32)(ETriangleNodeRelations::TNR_SAMEREGION);
	}
	if (IsNeighbour(Grid, TargetNode, OtherNode))
	{
		UKismetSystemLibrary::PrintString(Grid, FString("Neighbour"));
		Relations |= 1 << (int32)(ETriangleNodeRelations::TNR_NEIGHBOUR);
	}
	if (HasFreePath(Grid, TargetNode, OtherNode, TravelableTileTypes))
	{
		UKismetSystemLibrary::PrintString(Grid, FString("FreePath"));
		Relations |= 1 << (int32)(ETriangleNodeRelations::TNR_HASFREEPATH);
	}
	return Relations;
}
