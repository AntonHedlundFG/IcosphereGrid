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

	//Reduce region size by inserting subregions into the largest region
	//until all regions are smaller than Grid->GetMaxRegionSize()
	FLevelRegion LargestRegion = RemoveLargestRegion(Regions);
	while (LargestRegion.Size() > Grid->GetMaxRegionSize())
	{
		TArray<FLevelRegion> SubRegions = InsertSubRegion(LargestRegion);
		
		//Reset regions, in case new region borders on other region of same type.
		Regions.Empty();
		World.Region = TSet<UTriangleNode*>(Grid->GetNodes());
		Regions.Append(SortIntoRegionsByType(World));

		LargestRegion = RemoveLargestRegion(Regions);
	}
	Regions.Add(LargestRegion);
	
	UKismetSystemLibrary::PrintString(Grid, FString("Initial Regions: ") + FString::FromInt(Regions.Num()), true, false, FLinearColor::White, 10.0f);

	//Merge small regions into neighbouring regions
	MergeSmallRegionsIntoNeighbours(Regions, Grid->GetMinRegionSize());
	UKismetSystemLibrary::PrintString(Grid, FString("After Small Region Merge: ") + FString::FromInt(Regions.Num()), true, false, FLinearColor::White, 10.0f);

	//Merge water and hole regions since they can't be neighbours
	MergeWaterAndHoles(Grid);
	Regions.Empty();
	World.Region = TSet<UTriangleNode*>(Grid->GetNodes());
	Regions.Append(SortIntoRegionsByType(World));
	UKismetSystemLibrary::PrintString(Grid, FString("After WaterHole Merge: ") + FString::FromInt(Regions.Num()), true, false, FLinearColor::White, 10.0f);

}

void ULevelGeneration::MergeSmallRegionsIntoNeighbours(TArray<FLevelRegion>& Regions, int MinSize)
{
	for (int i = Regions.Num() - 1; i >= 0; i--)
	{
		if (Regions[i].Size() >= MinSize) continue;

		UTriangleNode* AnyNeighbour = nullptr;
		for (UTriangleNode* Node : Regions[i].Region)
		{
			for (UTriangleLink* Link : Node->GetLinks())
			{
				if (Regions[i].GetTileType() == Link->GetTarget()->GetTileType()) continue;

				AnyNeighbour = Link->GetTarget();
				break;
			}
			if (AnyNeighbour != nullptr) break;
		}
		if (AnyNeighbour == nullptr) return;

		for (FLevelRegion Region : Regions)
		{
			if (Regions[i] != Region && Region.Region.Contains(AnyNeighbour))
			{
				for (UTriangleNode* Node : Regions[i].Region)
				{
					Regions[i].Region.Remove(Node);
					Region.Region.Add(Node);
					Node->SetTileType(AnyNeighbour->GetTileType());
				}
				Regions.RemoveAt(i);
				break;
			}
		}

	}
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

void ULevelGeneration::MergeWaterAndHoles(AIcosphereGridActor* Grid)
{
	for (UTriangleNode* Node : Grid->GetNodes())
	{
		ETileType ThisType = Node->GetTileType();
		ETileType OtherType;
		if (ThisType == ETileType::TT_Water)
			OtherType = ETileType::TT_Hole;
		else if (ThisType == ETileType::TT_Hole)
			OtherType = ETileType::TT_Water;
		else continue;

		TQueue<UTriangleNode*> OpenSet;
		OpenSet.Enqueue(Node);
		UTriangleNode* Current;
		while (OpenSet.Dequeue(Current))
		{
			for (UTriangleLink* Link : Current->GetLinks())
			{
				if (Link->GetTarget()->GetTileType() == OtherType)
				{
					OpenSet.Enqueue(Link->GetTarget());
					Link->GetTarget()->SetTileType(ThisType);
				}
			}
		}
	}
}

FLevelRegion ULevelGeneration::RemoveLargestRegion(TArray<FLevelRegion>& Regions)
{
	FLevelRegion LargestRegion;
	for (FLevelRegion Region : Regions)
	{
		if (Region.Size() > LargestRegion.Size())
		{
			LargestRegion = Region;
		}
	}
	Regions.Remove(LargestRegion);
	return LargestRegion;
}

TArray<FLevelRegion> ULevelGeneration::InsertSubRegion(FLevelRegion Region)
{
	int NewRegionSize = FMath::RandRange(Region.Size() / 4, Region.Size() / 2);
	ETileType NewRegionType;
	do {
		NewRegionType = (ETileType)(FMath::RandRange(0, (int)(ETileType::TT_MAX) - 1));
	} while (NewRegionType == Region.GetTileType());
	TArray<UTriangleNode*> OpenSet;
	TSet<UTriangleNode*> ClosedSet;
	UTriangleNode* RandomStartNode = Region.Region.Array()[FMath::RandRange(0, Region.Region.Array().Num() - 1)];
	OpenSet.Add(RandomStartNode);
	while (!OpenSet.IsEmpty() && ClosedSet.Num() < NewRegionSize)
	{
		UTriangleNode* Current = OpenSet[FMath::RandRange(0, OpenSet.Num() - 1)];
		OpenSet.Remove(Current);
		ClosedSet.Add(Current);

		for (UTriangleLink* Link : Current->GetLinks())
		{
			UTriangleNode* Target = Link->GetTarget();
			if (Region.Region.Contains(Target) &&
				!OpenSet.Contains(Target) &&
				!ClosedSet.Contains(Target))
			{
				OpenSet.Add(Target);
			}
		}
	}
	
	for (UTriangleNode* Node : ClosedSet)
	{
		Node->SetTileType(NewRegionType);
	}


	return SortIntoRegionsByType(Region);
}