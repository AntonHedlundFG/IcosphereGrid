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

	/** Returns true if there is an intersection, and either way, gives the closest point on
	* the AABB from the Sphere.
	*/
	UFUNCTION(BlueprintCallable)
	static bool ClosestAABBIntersectPoint(FVector BoxCenter, FVector HalfExtents, FVector SphereCenter, float SphereRadius, FVector& ClosestPoint);

	UFUNCTION()
	static bool ContainedWithin(float Value, float Min, float Max);

	UFUNCTION()
	static bool ContainedWithinExtents(float Value, float Center, float HalfExtents);

	UFUNCTION()
	static float CalcGravitationalForce(float MassA, float MassB, float Radius);

	UFUNCTION()
	static float CalcGravitationalForceSqr(float MassA, float MassB, float SqrRadius);
};
