// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"


void ATankAIController::BeginPlay() {
	Super::BeginPlay();

//	auto PlayerTank = GetPlayerTank();

}


void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (ensure(PlayerTank && AimingComponent)){
		// TODO Move toward the player
		MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in centimeters
		// Aim toward the player
		AimingComponent->AimAt(PlayerTank->GetActorLocation());
		// Fire if ready
		AimingComponent->Fire(); // TODO limit firing rate
	}
}