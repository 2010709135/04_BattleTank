// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"


void ATankAIController::BeginPlay() {
	Super::BeginPlay();

//	auto PlayerTank = GetPlayerTank();

}


void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto ControlledTank = Cast<ATank>(GetPawn());

	if (PlayerTank) {
		// TODO Move toward the player
		MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in centimeters
		// Aim toward the player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		// Fire if ready
		//ControlledTank->Fire(); // TODO limit firing rate
	}
}