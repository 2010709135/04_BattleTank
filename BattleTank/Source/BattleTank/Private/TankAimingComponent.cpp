// Fill out your copyright notice in the Description page of Project Settings.
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed) {
	if (!Barrel) return;

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
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();

		/*auto OurTankName = GetOwner()->GetName();
		auto BarrelLocation = Barrel->GetComponentLocation().ToString();*/
		//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s"), *OurTankName, *AimDirection.ToString(), *BarrelLocation);
		
		//UE_LOG(LogTemp, Warning, TEXT("%s : found solution, direction : %s"),*GetName(), *AimDirection.ToString());

		MoveBarrelToward(AimDirection);
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("No solution"));

	}


	
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet) {
	if (!BarrelToSet) return;
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet) {
	if (!TurretToSet) return;
	Turret = TurretToSet;
}

void UTankAimingComponent::MoveBarrelToward(FVector AimDirection) {
	// work out difference between current barrel rotation and aim direction
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotation = AimAsRotator - BarrelRotation;
	// move the barrel the right amount this frame
	// given a max elevation speed, and the frame time
	Barrel->Elevate(DeltaRotation.Pitch);
	Turret->Rotate(DeltaRotation.Yaw);
}