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