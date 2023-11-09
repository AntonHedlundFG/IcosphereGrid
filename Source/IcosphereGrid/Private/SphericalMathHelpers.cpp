// Fill out your copyright notice in the Description page of Project Settings.


#include "SphericalMathHelpers.h"
#include "Kismet/KismetMathLibrary.h"

FVector USphericalMathHelpers::SlerpLocationAroundPoint(FVector Point, FVector Start, FVector End, float Alpha)
{
	FVector StartDelta = Start - Point;
	FVector EndDelta = End - Point;
	float Length = FMath::Lerp(StartDelta.Length(), EndDelta.Length(), Alpha);
	FVector NewNormalDelta = Alpha * StartDelta + (1.0f - Alpha) * EndDelta;
	NewNormalDelta.Normalize();
	FVector SlerpedLocation = NewNormalDelta * Length + Point;
	return SlerpedLocation;
}

FRotator USphericalMathHelpers::SlerpRotationAroundPoint(FVector Point, FVector Start, FVector End, float Alpha)
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

bool USphericalMathHelpers::GetRaySphereIntersect(FVector RayStart, FVector RayDirection, FVector SphereCenter, float SphereRadius, FVector& OutHitPoint)
{
    FVector RayStartToSphere = SphereCenter - RayStart;

    // Project RayStartToSphere on RayDirection
    float ClosestPointRayDistance = RayStartToSphere.Dot(RayDirection);

    // Ray starts outside sphere and points away from sphere 
    if (ClosestPointRayDistance < 0.0f && RayStartToSphere.Length() > SphereRadius) return false;

    // Calculate the distance from the point on the ray closest to the spheres center
    float ClosestPointSphereDistance = FMath::Sqrt(RayStartToSphere.Length() * RayStartToSphere.Length() - ClosestPointRayDistance * ClosestPointRayDistance);

    // If the closest point on the ray is greater than the sphere's radius, there is no intersection
	if (ClosestPointSphereDistance > SphereRadius) return false;

    // Calculate the intersection point
    float IntersectionDistance = ClosestPointRayDistance - FMath::Sqrt(SphereRadius * SphereRadius - ClosestPointSphereDistance * ClosestPointSphereDistance);
    OutHitPoint = RayStart + RayDirection * IntersectionDistance;

    return true;
}