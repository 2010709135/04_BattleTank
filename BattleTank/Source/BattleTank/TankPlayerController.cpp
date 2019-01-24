// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"

#define OUT

ATank* ATankPlayerController::GetControlledTank() const 
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();

	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not posseing Play : %s"), *(GetPawn()->GetName()));
	}else
		UE_LOG(LogTemp, Warning, TEXT("PlayerController posseing : %s"), *(ControlledTank->GetName()));

}

void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair() 
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; // Out parameter

	if (GetSightRayHitLocation(HitLocation)) { // Has "side-effect', is going to line trace
		//UE_LOG(LogTemp, Warning, TEXT("HitLocation : %s"), *(HitLocation.ToString()));
		GetControlledTank()->AimAt(HitLocation);
		// TODO Tell controlled tank to ail at this point
	}


	
}

// Get world location if linetrace throught crosshair, true if hits landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const 
{
	int32 ViewportSizeX, ViewportSizeY;

	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("Look Direction : %s"), *(LookDirection.ToString()));
		//bool GetLookVectorHitLocation() const;
		GetLookVectorHitLocation(LookDirection, OutHitLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Look Direction : %s"), *(LookDirection.ToString()));

	}

	return true;
}

bool ATankPlayerController::GetLookDirection(FVector2D screenLocation, FVector& LookDirection) const {
	FVector CameraWorldLocation;
	return DeprojectScreenPositionToWorld(
		screenLocation.X, 
		screenLocation.Y, 
		CameraWorldLocation, 
		LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const {
	FHitResult HitResult;

	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility
	))
	{
		HitLocation = HitResult.Location;
		return true;
	}
	else
	{
		HitLocation = FVector(0);
		return false;
	}

}
