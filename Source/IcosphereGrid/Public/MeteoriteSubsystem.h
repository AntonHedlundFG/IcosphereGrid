// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MeteoriteSubsystem.generated.h"

class ABouncyMeteorite;
class AIcosphereGridActor;

/**
 * 
 */
UCLASS()
class ICOSPHEREGRID_API UMeteoriteSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	//Apparantly required for ticking?
	virtual TStatId GetStatId() const override { return GetStatID(); }

	virtual void Tick(float DeltaTime) override;

#pragma region Actor Registration

	UFUNCTION()
	void RegisterMeteorite(ABouncyMeteorite* Meteorite) { Meteorites.AddUnique(Meteorite); }
	
	UFUNCTION()
	void UnregisterMeteorite(ABouncyMeteorite* Meteorite) { Meteorites.Remove(Meteorite); }

	UFUNCTION()
	void RegisterPlanet(AIcosphereGridActor* Planet) { Planets.AddUnique(Planet); }

	UFUNCTION()
	void UnregisterPlanet(AIcosphereGridActor* Planet) { Planets.Remove(Planet); }

protected:

	//A list of all registered meteorites
	UPROPERTY()
	TArray<ABouncyMeteorite*> Meteorites;

	//A list of all registered planets
	UPROPERTY()
	TArray<AIcosphereGridActor*> Planets;

#pragma endregion

};
