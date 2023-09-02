// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TriangleLink.generated.h"

class UTriangleNode;
class AIcosphereGridActor;

/**
 * 
 */
UCLASS()
class ICOSPHEREGRID_API UTriangleLink : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetLinks(AIcosphereGridActor* ParentGrid, UTriangleNode* Source, UTriangleNode* Target);

	UFUNCTION()
	UTriangleNode* GetSource() { return SourceNode; }
	
	UFUNCTION()
	UTriangleNode* GetTarget() { return TargetNode; }

	UFUNCTION()
	AIcosphereGridActor* GetGrid() { return Grid; }

protected:
	UPROPERTY()
	TObjectPtr<UTriangleNode> SourceNode;

	UPROPERTY()
	TObjectPtr<UTriangleNode> TargetNode;

	UPROPERTY()
	TObjectPtr<AIcosphereGridActor> Grid;
};
