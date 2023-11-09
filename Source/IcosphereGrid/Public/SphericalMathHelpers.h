// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SphericalMathHelpers.generated.h"

/**
 * 
 */
UCLASS()
class ICOSPHEREGRID_API USphericalMathHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/** This determines locations on a globe, defined by Point, by spherically
	* interpolating between Start and End.
	*/
	UFUNCTION(BlueprintCallable)
	static FVector SlerpLocationAroundPoint(FVector Point, FVector Start, FVector End, float Alpha);

	/** This determines rotations on a globe, defined by Point, by spherically
	* interpolating between Start and End.
	* The up axis is the normal of the globe's surface at our current location
	*/
	UFUNCTION(BlueprintCallable)
	static FRotator SlerpRotationAroundPoint(FVector Point, FVector Start, FVector End, float Alpha);

	UFUNCTION(BlueprintCallable)
	static bool GetRaySphereIntersect(FVector RayStart, FVector RayDirection, FVector SphereCenter, float SphereRadius, FVector& OutHitPoint);
};
