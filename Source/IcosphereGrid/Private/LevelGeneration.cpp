// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGeneration.h"
#include "IcosphereGridActor.h"
#include "TriangleLink.h"
#include "Kismet/KismetSystemLibrary.h"

void ULevelGeneration::GenerateLevel(AIcosphereGridActor* Grid, FLevelGenerationSettings Settings)
{
	//Establish list of all starting regions by type.
	//Note that World can contain many diffent types, but once Regions has been
	//created we should keep regions sorted by type from now on.
	FLevelRegion World;
	World.Region = TSet<UTriangleNode*>(Grid->GetNodes());
	TArray<FLevelRegion> Regions;
	Regions.Append(SortIntoRegionsByType(World));
}

TArray<FLevelRegion> ULevelGeneration::SortIntoRegionsByType(FLevelRegion DivideRegion)
{
	TSet<UTriangleNode*> StartRegion = DivideRegion.Region;

	TArray<FLevelRegion> ReturnArray;
	while (!StartRegion.IsEmpty())
	{
		TQueue<UTriangleNode*> OpenSet;
		UTriangleNode* StartNode = StartRegion.Array()[0];
		OpenSet.Enqueue(StartNode);
		StartRegion.Remove(StartNode);
		FLevelRegion CurrentRegion;
		CurrentRegion.Region.Add(StartNode);
		UTriangleNode* Current;
		while (OpenSet.Dequeue(Current))
		{

			for (UTriangleLink* Link : Current->GetLinks())
			{
				if (StartRegion.Contains(Link->GetTarget()) &&
					Link->GetTarget()->GetTileType() == StartNode->GetTileType())
				{
					StartRegion.Remove(Link->GetTarget());
					CurrentRegion.Region.Add(Link->GetTarget());
					OpenSet.Enqueue(Link->GetTarget());
				}
			}
		}
		ReturnArray.Add(CurrentRegion);
	}

	return ReturnArray;
}


FLevelRegion ULevelGeneration::RemoveLargestRegion(TArray<FLevelRegion> Regions)
{
	int LargestSize = -1;
	FLevelRegion LargestRegion;
	for (FLevelRegion Region : Regions)
	{
		if (Region.Region.Num() > LargestSize)
		{
			LargestRegion = Region;
			LargestSize = Region.Region.Num();
		}
	}
	Regions.Remove(LargestRegion);
	return LargestRegion;
}