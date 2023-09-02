// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleLink.h"

void UTriangleLink::SetLinks(AIcosphereGridActor* ParentGrid, UTriangleNode* Source, UTriangleNode* Target)
{
	Grid = ParentGrid;
	SourceNode = Source;
	TargetNode = Target;
}