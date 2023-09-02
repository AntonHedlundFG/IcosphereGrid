// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TriangleLink.generated.h"

class UTriangleNode;


/**
 * 
 */
UCLASS()
class ICOSPHEREGRID_API UTriangleLink : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetLinks(UTriangleNode* Source, UTriangleNode* Target);

	UFUNCTION()
	UTriangleNode* GetSource() const { return SourceNode; }
	
	UFUNCTION()
	UTriangleNode* GetTarget() const { return TargetNode; }

protected:
	UPROPERTY()
	TObjectPtr<UTriangleNode> SourceNode;

	UPROPERTY()
	TObjectPtr<UTriangleNode> TargetNode;
};
