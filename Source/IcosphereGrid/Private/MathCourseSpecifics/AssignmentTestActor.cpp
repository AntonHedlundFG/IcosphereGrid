// Fill out your copyright notice in the Description page of Project Settings.


#include "MathCourseSpecifics/AssignmentTestActor.h"
#include "TriangleNodeRelations.h"
#include "IcosphereGridActor.h"
#include "TriangleNode.h"
#include "TriangleLink.h"
#include "SphericalMathHelpers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "BouncyMeteorite.h"


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

	ExerciseThreeTick(DeltaTime);

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

	//Generate start & stop location located slightly above the globe's surface.
	LocationStart = AllNodes[StartIndex]->GetCenterPosition();
	LocationStart *= 1.1f;
	LocationStart += Grid->GetActorLocation();
	LocationStop = AllNodes[StopIndex]->GetCenterPosition();
	LocationStop *= 1.1f;
	LocationStop += Grid->GetActorLocation();

	//Set to active so ExerciseTwoTick starts running, with a small delay
	CurrentMoveTime = -StartDelay;
	bTwoIsActive = true;

	//Debug-Draw the trajectory
	for (float f = 0.0f; f < 100.0f; f += 1.0f)
	{
		FVector LineStart = USphericalMathHelpers::SlerpLocationAroundPoint(Grid->GetActorLocation(), LocationStart, LocationStop, f / 100.0f);
		FVector LineEnd = USphericalMathHelpers::SlerpLocationAroundPoint(Grid->GetActorLocation(), LocationStart, LocationStop, (f + 1.0f) / 100.0f);
		UKismetSystemLibrary::DrawDebugLine(this, LineStart, LineEnd, FLinearColor::Black, StartDelay + MoveDuration);;
	}

}

void AAssignmentTestActor::ExerciseTwoTick(float DeltaTime)
{
	if (!bTwoIsActive) return;

	CurrentMoveTime += DeltaTime;
	
	//We're still in the start delay
	if (CurrentMoveTime < 0.0f) return;

	//End activity if we've completed the move
	if (CurrentMoveTime >= MoveDuration)
	{
		bTwoIsActive = false;
		CurrentMoveTime = MoveDuration;
	}

	//Use the helper functions to actually set the units position & rotation.
	const float Alpha = CurrentMoveTime / MoveDuration;
	MovableUnit->SetActorLocation(
		USphericalMathHelpers::SlerpLocationAroundPoint(Grid->GetActorLocation(), LocationStart, LocationStop, Alpha)
	);
	MovableUnit->SetActorRotation(
		USphericalMathHelpers::SlerpRotationAroundPoint(Grid->GetActorLocation(), LocationStart, LocationStop, Alpha)
	);

}

void AAssignmentTestActor::ExerciseThreeTick(float DeltaTime)
{
	if (!bThreeIsActive) return;

	//Find the PlayerControllers view, to establish the ray that we will use to intersect the grid.
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	int32 X, Y;
	PC->GetViewportSize(X, Y);
	FVector2D ViewportSize = FVector2D(X, Y);
	FVector WorldPosition, WorldDirection;
	UGameplayStatics::DeprojectScreenToWorld(PC, ViewportSize * 0.5f, WorldPosition, WorldDirection);
	FVector OutHitPoint;

	//Ask the grid for the node that our camera view is intersecting.
	UTriangleNode* Node = Grid->GetNodeFromRaycast(WorldPosition, WorldDirection);
	if (!Node) return;

	//Get the mid point and vertex locations in world space, elevated slightly for visibility
	//as some meshes are already elevated.
	FVector NodeMid = Grid->GetActorLocation() + Node->GetCenterPosition() * 1.06f;
	TArray<FVector> Vertices;
	for (FVector Vertex : Node->GetVertices())
		Vertices.Add(Vertex * 1.02f + Grid->GetActorLocation());

	//Draw a debug pyramid displaying the intersected node
	for (int i = 0; i < Vertices.Num(); i++)
	{
		UKismetSystemLibrary::DrawDebugLine(this, NodeMid, Vertices[i], FLinearColor::White, 0.0f, 5.0f);
		UKismetSystemLibrary::DrawDebugLine(this, Vertices[i], Vertices[(i + 1) % Vertices.Num()], FLinearColor::White, 0.0f, 5.0f);
	}
	
}

void AAssignmentTestActor::SpawnMeteorite()
{
	for (int i = 0; i < SpawnsPerClick; i++)
	{
		//Spawn the meteorite 10-100 percent of the planets radius above the surface.
		FVector SpawnDirection = UKismetMathLibrary::RandomUnitVector();
		float SpawnDistance = Grid->GetRadius() * FMath::RandRange(1.1f, 2.0f);
		const FVector SpawnLocation = Grid->GetActorLocation() + SpawnDirection * SpawnDistance;
		FActorSpawnParameters SpawnParams;
		ABouncyMeteorite* SpawnedMeteorite = GetWorld()->SpawnActor<ABouncyMeteorite>(
			MeteoriteClass,
			SpawnLocation,
			FRotator::ZeroRotator,
			SpawnParams);


		//Aim towards the centre of the planet, and then offset the direction randomly.
		//Randomly set the velocity, as well as the mass.
		FVector AimDirection = -SpawnDirection;
		AimDirection += FVector(FMath::RandRange(-0.3f, 0.6f),
			FMath::RandRange(-0.6f, 0.6f),
			FMath::RandRange(-0.6f, 0.6f));
		FVector StartingVelocity = AimDirection * MeteoriteSpeed * FMath::RandRange(0.5f, 1.5f);
		SpawnedMeteorite->SetVelocity(StartingVelocity);
		SpawnedMeteorite->SetMass(SpawnedMeteorite->GetMass() * FMath::RandRange(0.5f, 1.5f));
	}

}