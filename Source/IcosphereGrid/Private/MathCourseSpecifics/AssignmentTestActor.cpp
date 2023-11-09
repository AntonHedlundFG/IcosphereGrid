// Fill out your copyright notice in the Description page of Project Settings.


#include "MathCourseSpecifics/AssignmentTestActor.h"
#include "TriangleNodeRelations.h"
#include "IcosphereGridActor.h"
#include "TriangleNode.h"
#include "TriangleLink.h"
#include "Kismet/KismetSystemLibrary.h"

AAssignmentTestActor::AAssignmentTestActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AAssignmentTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAssignmentTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAssignmentTestActor::ExerciseOneTest()
{
	//Make sure we have a grid and that it contains more than 1 node
	if (!Grid) return;
	TArray<UTriangleNode*> AllNodes = Grid->GetNodes();
	if (AllNodes.Num() < 2) return;

	//Get two random indices, but they can't be the same one.
	int TargetIndex, OtherIndex;
	TargetIndex = FMath::RandRange(0, AllNodes.Num() - 1);
	do {
		OtherIndex = FMath::RandRange(0, AllNodes.Num() - 1);
	} while (OtherIndex == TargetIndex && AllNodes.Num() >= 2);

	TriangleNodeRelationsFlags = UTriangleNodeRelations::GetNodeRelations(
		Grid, 
		AllNodes[TargetIndex], 
		AllNodes[OtherIndex], 
		TravelableTileTypes
	);
	UKismetSystemLibrary::PrintString(this, FString::FromInt(TriangleNodeRelationsFlags), true, false, FLinearColor::Red, 30.0f);

	UKismetSystemLibrary::DrawDebugSphere(this, Grid->GetActorLocation() + AllNodes[TargetIndex]->GetCenterPosition(), 50.0f, 12, FLinearColor::Blue, 30.0f);
	UKismetSystemLibrary::DrawDebugLine(this, Grid->GetActorLocation() + AllNodes[TargetIndex]->GetCenterPosition(), Grid->GetActorLocation() + 2.0f * AllNodes[TargetIndex]->GetCenterPosition(), FLinearColor::Blue, 30.0f);
	
	UKismetSystemLibrary::DrawDebugSphere(this, Grid->GetActorLocation() + AllNodes[OtherIndex]->GetCenterPosition(), 50.0f, 12, FLinearColor::Red, 30.0f);
	UKismetSystemLibrary::DrawDebugLine(this, Grid->GetActorLocation() + AllNodes[OtherIndex]->GetCenterPosition(), Grid->GetActorLocation() + 2.0f * AllNodes[OtherIndex]->GetCenterPosition(), FLinearColor::Red, 30.0f);
}
