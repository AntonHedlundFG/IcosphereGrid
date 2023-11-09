// Fill out your copyright notice in the Description page of Project Settings.


#include "MathCourseSpecifics/AssignmentTestActor.h"
#include "TriangleNodeRelations.h"
#include "IcosphereGridActor.h"
#include "TriangleNode.h"
#include "TriangleLink.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


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

	ExerciseTwoTick(DeltaTime);

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

	UKismetSystemLibrary::DrawDebugSphere(this, Grid->GetActorLocation() + AllNodes[TargetIndex]->GetCenterPosition(), 50.0f, 12, FLinearColor::Blue, 30.0f);
	UKismetSystemLibrary::DrawDebugLine(this, Grid->GetActorLocation() + AllNodes[TargetIndex]->GetCenterPosition(), Grid->GetActorLocation() + 2.0f * AllNodes[TargetIndex]->GetCenterPosition(), FLinearColor::Blue, 30.0f);
	UKismetSystemLibrary::DrawDebugSphere(this, Grid->GetActorLocation() + AllNodes[OtherIndex]->GetCenterPosition(), 50.0f, 12, FLinearColor::Red, 30.0f);
	UKismetSystemLibrary::DrawDebugLine(this, Grid->GetActorLocation() + AllNodes[OtherIndex]->GetCenterPosition(), Grid->GetActorLocation() + 2.0f * AllNodes[OtherIndex]->GetCenterPosition(), FLinearColor::Red, 30.0f);
}

void AAssignmentTestActor::ExerciseTwoTest()
{
	//Make sure we have a grid and that it contains more than 1 node
	if (!Grid) return;
	TArray<UTriangleNode*> AllNodes = Grid->GetNodes();
	if (AllNodes.Num() < 2) return;

	//Get two random indices, but they can't be the same one.
	int StartIndex, StopIndex;
	StartIndex = FMath::RandRange(0, AllNodes.Num() - 1);
	do {
		StopIndex = FMath::RandRange(0, AllNodes.Num() - 1);
	} while (StopIndex == StartIndex && AllNodes.Num() >= 2);

	LocationStart = AllNodes[StartIndex]->GetCenterPosition();
	LocationStart *= 1.1f;
	LocationStart += Grid->GetActorLocation();
	LocationStop = AllNodes[StopIndex]->GetCenterPosition();
	LocationStop *= 1.1f;
	LocationStop += Grid->GetActorLocation();

	UpStart = AllNodes[StartIndex]->GetUpDirection();
	UpStop = AllNodes[StopIndex]->GetUpDirection();

	//After this, we can determine the forward vector
	//by taking the cross product of the lerped Up vector and the RightVector
	RightDirection = (LocationStop - LocationStart).Cross(UpStart);

	CurrentMoveTime = -StartDelay;
	bIsActive = true;

	UKismetSystemLibrary::DrawDebugSphere(this, LocationStart, 50.0f, 12, FLinearColor::Blue, 30.0f);
	UKismetSystemLibrary::DrawDebugSphere(this, LocationStop, 50.0f, 12, FLinearColor::Red, 30.0f);

}

void AAssignmentTestActor::ExerciseTwoTick(float DeltaTime)
{
	if (!bIsActive) return;

	CurrentMoveTime += DeltaTime;
	
	if (CurrentMoveTime < 0.0f) return;

	if (CurrentMoveTime >= MoveDuration)
	{
		bIsActive = false;
		CurrentMoveTime = MoveDuration;
	}

	const float Alpha = CurrentMoveTime / MoveDuration;
	MovableUnit->SetActorLocation(
		SlerpLocationAroundPoint(Grid->GetActorLocation(), LocationStart, LocationStop, Alpha)
	);
	MovableUnit->SetActorRotation(
		SlerpRotationAroundPoint(Grid->GetActorLocation(), LocationStart, LocationStop, Alpha)
	);

	//Debug-Drawing the whole trajectory

	for (float f = 0.0f; f < 100.0f; f += 1.0f)
	{
		FVector LineStart = SlerpLocationAroundPoint(Grid->GetActorLocation(), LocationStart, LocationStop, f / 100.0f);
		FVector LineEnd = SlerpLocationAroundPoint(Grid->GetActorLocation(), LocationStart, LocationStop, (f + 1.0f) / 100.0f);
		UKismetSystemLibrary::DrawDebugLine(this, LineStart, LineEnd, FLinearColor::Black);
	}

}

FVector AAssignmentTestActor::SlerpLocationAroundPoint(FVector Point, FVector Start, FVector End, float Alpha)
{
	FVector StartDelta = Start - Point;
	FVector EndDelta = End - Point;
	float Length = FMath::Lerp(StartDelta.Length(), EndDelta.Length(), Alpha);
	FVector NewNormalDelta = Alpha * StartDelta + (1.0f - Alpha) * EndDelta;
	NewNormalDelta.Normalize();
	FVector SlerpedLocation = NewNormalDelta * Length + Point;
	return SlerpedLocation;
}

FRotator AAssignmentTestActor::SlerpRotationAroundPoint(FVector Point, FVector Start, FVector End, float Alpha)
{
	FVector StartDelta = Start - Point;
	FVector EndDelta = End - Point;

	FVector Forward, Right, Up;

	FVector CurrentLocation = SlerpLocationAroundPoint(Point, Start, End, Alpha);
	FVector CurrentDelta = CurrentLocation - Point;
	Up = CurrentDelta.GetSafeNormal();
	
	Right = (EndDelta - StartDelta).GetSafeNormal().Cross(Up).GetSafeNormal();
	Forward = Right.Cross(Up).GetSafeNormal();

	return UKismetMathLibrary::MakeRotationFromAxes(Forward, Right, Up);
}
