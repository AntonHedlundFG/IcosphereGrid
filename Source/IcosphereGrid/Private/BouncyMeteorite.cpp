// Fill out your copyright notice in the Description page of Project Settings.


#include "BouncyMeteorite.h"
#include "MeteoriteSubsystem.h"
#include "Components/SphereComponent.h"

void ABouncyMeteorite::PhysicsTick(float DeltaTime)
{
	SetActorLocation(GetActorLocation() + Velocity * DeltaTime);
}

void ABouncyMeteorite::ApplyForce(FVector Force)
{
	Velocity += Force / Mass;
}

float ABouncyMeteorite::GetRadius()
{
	return Sphere->GetScaledSphereRadius();
}

float ABouncyMeteorite::GetRadiusSqr()
{
	return Sphere->GetScaledSphereRadius() * Sphere->GetScaledSphereRadius();
}

// Sets default values
ABouncyMeteorite::ABouncyMeteorite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	Sphere = CreateDefaultSubobject<USphereComponent>(FName("RootSphere"));
	SetRootComponent(Sphere);
}

// Called when the game starts or when spawned
void ABouncyMeteorite::BeginPlay()
{
	Super::BeginPlay();

	UMeteoriteSubsystem* MSubSystem = GetWorld()->GetSubsystem<UMeteoriteSubsystem>();
	if (MSubSystem)
	{
		MSubSystem->RegisterMeteorite(this);
	}
}

void ABouncyMeteorite::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		UMeteoriteSubsystem* MSubSystem = GetWorld()->GetSubsystem<UMeteoriteSubsystem>();
		if (MSubSystem)
		{
			MSubSystem->UnregisterMeteorite(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ABouncyMeteorite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

