// Fill out your copyright notice in the Description page of Project Settings.


#include "MeteoriteSubsystem.h"
#include "BouncyMeteorite.h"
#include "IcosphereGridActor.h"
#include "SphericalMathHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

#define PRINT(msg) UKismetSystemLibrary::PrintString(this, msg, true, false, FLinearColor::White, 10.0f);

void UMeteoriteSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Apply all gravitational forces to meteorites
	for (ABouncyMeteorite* Meteorite : Meteorites)
	{
		if (!Meteorite->bIsGravityEnabled) continue;

		//Gravity from other meteorites
		for (ABouncyMeteorite* Other : Meteorites)
		{
			if (Other == Meteorite || !Other->bIsGravityEnabled) continue;

			//Calculate the force using the equation for universal gravitation
			float Force = USphericalMathHelpers::CalcGravitationalForceSqr(
				Meteorite->GetMass(),
				Other->GetMass(),
				FVector::DistSquared(Other->GetActorLocation(), Meteorite->GetActorLocation())
			);

			//Since we're applying gravity each tick, multiply the force by DeltaTime.
			Meteorite->ApplyForce(
				(Other->GetActorLocation() - Meteorite->GetActorLocation())
				.GetSafeNormal() * Force * DeltaTime);
		}

		//Gravity from planets
		for (AIcosphereGridActor* Planet : Planets)
		{
			//Calculate the force using the equation for universal gravitation
			float Force = USphericalMathHelpers::CalcGravitationalForceSqr(
				Meteorite->GetMass(),
				Planet->GetMass(),
				FVector::DistSquared(Planet->GetActorLocation(), Meteorite->GetActorLocation())
			);

			//Since we're applying gravity each tick, multiply the force by DeltaTime.
			Meteorite->ApplyForce(
				(Planet->GetActorLocation() - Meteorite->GetActorLocation())
				.GetSafeNormal() * Force * DeltaTime);
		}
	}

	//Move the meteorites
	for (ABouncyMeteorite* Meteorite : Meteorites)
	{
		Meteorite->PhysicsTick(DeltaTime);
	}


	//Here comes the fun part, checking for and handling Sphere-to-Sphere collisions
	for (ABouncyMeteorite* Meteorite : Meteorites)
	{
		for (ABouncyMeteorite* Other : Meteorites)
		{
			if (Other == Meteorite) continue;

			//Check if we are actually colliding by comparing radii with distance.
			float DistSquared = FVector::DistSquared(Meteorite->GetActorLocation(), Other->GetActorLocation());
			if (DistSquared > Meteorite->GetRadiusSqr() + Other->GetRadiusSqr())
				continue;

			//Determine collision normal and how large the overlap is
			FVector Normal = (Meteorite->GetActorLocation() - Other->GetActorLocation()).GetSafeNormal();
			float OverlappedDistance = Meteorite->GetRadius() + Other->GetRadius()
				- FVector::Distance(Meteorite->GetActorLocation(), Other->GetActorLocation());

			//We determine the relative velocities of the bodies.
			//However, we only need to take into account their velocity along their normal
			//so we project their relative velocities onto the normal with a dot product.
			FVector RelativeVelocity = Other->GetMeteoriteVelocity() - Meteorite->GetMeteoriteVelocity();
			float RelevantSpeed = FVector::DotProduct(RelativeVelocity, Normal);
			RelevantSpeed = FMath::Abs(RelevantSpeed);

			//Bastardized version of the law of conservation of momentum. Seems to work.. Suspicious!
			float TotalForce = RelevantSpeed * (Meteorite->GetMass() + Other->GetMass());
			Meteorite->ApplyForce(Normal * TotalForce);
			Other->ApplyForce(-Normal * TotalForce);

			//Finally, once the forces have been applied we need to actually separate the overlapping bodies.
			//We let the heavier object move less.
			float MassPercentage = Meteorite->GetMass() / (Other->GetMass() + Meteorite->GetMass());
			Meteorite->SetActorLocation(
				Meteorite->GetActorLocation() +
				Normal * OverlappedDistance * (1.0f - MassPercentage)
			);
			Other->SetActorLocation(
				Other->GetActorLocation() -
				Normal * OverlappedDistance * MassPercentage
			);

		}

		for (AIcosphereGridActor* Planet : Planets)
		{
			//Check if we are actually colliding by comparing radii with distance.
			float DistSquared = FVector::DistSquared(Meteorite->GetActorLocation(), Planet->GetActorLocation());
			if (DistSquared > Meteorite->GetRadiusSqr() + Planet->GetRadiusSqr())
				continue;

			//Determine collision normal and how large the overlap is
			FVector Normal = (Meteorite->GetActorLocation() - Planet->GetActorLocation()).GetSafeNormal();
			float OverlappedDistance = Meteorite->GetRadius() + Planet->GetRadius()
				- FVector::Distance(Meteorite->GetActorLocation(), Planet->GetActorLocation());

			//In this case, since only the meteorite is moving, we only need to project it onto the normal.
			FVector RelativeVelocity = Meteorite->GetMeteoriteVelocity();
			float RelevantSpeed = FVector::DotProduct(RelativeVelocity, Normal);
			RelevantSpeed = FMath::Abs(RelevantSpeed);

			//Here we don't really care about momentum, we just want to reflect the velocity.
			Meteorite->SetVelocity(Meteorite->GetVelocity() + Normal * RelevantSpeed);

			//In the case of planets, only the meteorite moves; as the planet is stationary.
			Meteorite->SetActorLocation(
				Meteorite->GetActorLocation() +
				Normal * OverlappedDistance
			);

		}
	}

}
