// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssignmentTestActor.generated.h"

class AIcosphereGridActor;
class ABouncyMeteorite;
enum class ETileType : uint8;

UCLASS( hidecategories=(Movement, Input, Collision, Rendering, HLOD, WorldPartition, DataLayers, Replication, Physics, Networking, Actor, LevelInstance, Cooking))
class ICOSPHEREGRID_API AAssignmentTestActor : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "All Exercises")
	TObjectPtr<AIcosphereGridActor> Grid;

#pragma region Exercise One

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise One")
	TArray<ETileType> TravelableTileTypes;

	/** I use this method to do tests in runtime, as the grid is generated
	*	upon game launch and not accessible in non-play editor mode.
	*/
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Exercise One")
	void ExerciseOneTest();

	/** This is generated by using the ExerciseOneTest function.
	*	It shows the relations between two randomly selected TriangleNodes
	*	on the targeted Grid. The target node is shown with a blue debug sphere,
	*	the other node with a red sphere.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exercise One", meta = (Bitmask, BitmaskEnum = "/Script/IcosphereGrid.ETriangleNodeRelations"))
	int32 TriangleNodeRelationsFlags;

#pragma endregion

#pragma region Exercise Two

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise Two")
	TObjectPtr<AActor> MovableUnit;

	/** I use this method to do tests in runtime, as the grid is generated
	*	upon game launch and not accessible in non-play editor mode.
	*/
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Exercise Two")
	void ExerciseTwoTest();

	/**We call this from Tick so we can separate different exercises' tick functions.
	*/
	UFUNCTION()
	void ExerciseTwoTick(float DeltaTime);

	/**The time it takes to move across the trajectory
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise Two")
	float MoveDuration = 5.0f;

	/** A short delay before the movement starts so we can find the trajectory in editor
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise Two")
	float StartDelay = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exercuse Two")
	float CurrentMoveTime = 0.0f;

	UPROPERTY()
	bool bTwoIsActive = false;

	//We store these two locations here since we assign them once and then
	//need to access them in ExerciseTwoTick()
	UPROPERTY() FVector LocationStart;
	UPROPERTY() FVector LocationStop;

#pragma endregion

#pragma region Exercise Three

protected:
	UFUNCTION()
	void ExerciseThreeTick(float DeltaTime);

	/** When activated, will display a debug pyramid on the node which
	* is currently in the center of the viewport.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise Three")
	bool bThreeIsActive;

#pragma endregion

#pragma region Exercise Four

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Exercise Four")
	void SpawnMeteorite();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise Four")
	int SpawnsPerClick = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise Four")
	float MeteoriteSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise Four")
	TSubclassOf<ABouncyMeteorite> MeteoriteClass;

#pragma endregion

public:	
	AAssignmentTestActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


};
