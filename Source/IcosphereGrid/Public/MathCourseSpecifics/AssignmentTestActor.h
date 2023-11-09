// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssignmentTestActor.generated.h"

class AIcosphereGridActor;
enum class ETileType : uint8;

UCLASS( hidecategories=(Movement, Input, Collision, Rendering, HLOD, WorldPartition, DataLayers, Replication, Physics, Networking, Actor, LevelInstance, Cooking))
class ICOSPHEREGRID_API AAssignmentTestActor : public AActor
{
	GENERATED_BODY()


#pragma region Exercise One

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Exercise One")
	TObjectPtr<AIcosphereGridActor> Grid;

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


#pragma endregion

public:	
	AAssignmentTestActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


};
