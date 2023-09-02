// Fill out your copyright notice in the Description page of Project Settings.


#include "TriangleLink.h"

void UTriangleLink::SetLinks(UTriangleNode* Source, UTriangleNode* Target)
{
	SourceNode = Source;
	TargetNode = Target;
}