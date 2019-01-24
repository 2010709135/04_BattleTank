// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "GameFramework/Actor.h"
#include "math.h"

void UTankBarrel::Elevate(float RelativeSpeed) {
	// move the barrel the right amount this frame
// given a max elevation speed, and the frame time
	//auto Time = GetWorld()->GetTimeSeconds();
	//UE_LOG(LogTemp, Warning, TEXT("%f : Barrel->Elevate(%f)"), Time, RelativeSpeed);

	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);

	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;

	RawNewElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);
	SetRelativeRotation(FRotator(RawNewElevation, 0, 0));
}


