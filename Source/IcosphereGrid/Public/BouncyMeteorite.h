// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BouncyMeteorite.generated.h"

class USphereComponent;

UCLASS( hidecategories=(Input, Movement, Collision, Rendering, HLOD, WorldPartition, DataLayers, Replication, Physics, Networking, Actor, LevelInstance, Cooking))
class ICOSPHEREGRID_API ABouncyMeteorite : public AActor
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void PhysicsTick(float DeltaTime);

	UFUNCTION()
	void ApplyForce(FVector Force);

	UFUNCTION()
	float GetRadius();

	UFUNCTION()
	float GetRadiusSqr();

	UFUNCTION()
	void SetVelocity(FVector NewVelocity) { Velocity = NewVelocity; }

	UFUNCTION()
	FVector GetMeteoriteVelocity() { return Velocity; }

	UFUNCTION()
	float GetMass() { return Mass; }

	UFUNCTION()
	void SetMass(float NewMass) { Mass = NewMass; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Exercise Four")
	bool bIsGravityEnabled = true;
	
protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Exercise Four", meta = (ClampMin = 0.0f))
	float Mass = 100.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Exercise Four")
	FVector Velocity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Exercuse Four")
	TObjectPtr<USphereComponent> Sphere;


public:	
	ABouncyMeteorite();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
