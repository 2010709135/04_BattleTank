// Fill out your copyright notice in the Description page of Project Settings.
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay() {
	LastFireTime = FPlatformTime::Seconds();
	AimDirection = FVector(0);
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
	if (RoundsLeft <= 0) {
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds) {
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving()) {
		FiringState = EFiringState::Amiming;

	}
	else {
		FiringState = EFiringState::Locked;

	}
}



void UTankAimingComponent::AimAt(FVector HitLocation) {
	if (!ensure(Barrel)) return;

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	   	
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);



	if (bHaveAimSolution) {
		// Calculate the OutLaunchVelocity
		AimDirection = OutLaunchVelocity.GetSafeNormal();

		MoveBarrelToward(AimDirection);
	}
	else {

	}	
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet) {
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

bool UTankAimingComponent::IsBarrelMoving() {
	if (!ensure(Barrel)) return false;

	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.1);
}

void UTankAimingComponent::MoveBarrelToward(FVector AimDirection) {
	if (!ensure(Barrel) || !ensure(Turret)) return;
	// work out difference between current barrel rotation and aim direction
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotation = AimAsRotator - BarrelRotation;
	// move the barrel the right amount this frame
	// given a max elevation speed, and the frame time
	Barrel->Elevate(DeltaRotation.Pitch);
	if (FMath::Abs(DeltaRotation.Yaw) < 180) {
		Turret->Rotate(DeltaRotation.Yaw);
	}
	else {
		Turret->Rotate(-DeltaRotation.Yaw);
	}
}

void UTankAimingComponent::Fire() {


	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Amiming) {

		if (!ensure(Barrel && ProjectileBlueprint)) return;


		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int UTankAimingComponent::GetRoundsLeft() const {
	return RoundsLeft;
}