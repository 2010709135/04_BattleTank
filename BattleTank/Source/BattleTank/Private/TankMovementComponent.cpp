// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"
#include "math.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet) {
	if (!LeftTrackToSet || !RightTrackToSet) return;
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw) {
	if (!ensure(LeftTrack && RightTrack)) return;
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

}

void UTankMovementComponent::IntendTurnRight(float Throw) {
	if (!ensure(LeftTrack && RightTrack)) return;
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);

}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) 
{
	// No need to call Super as we're replacing the functionality
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal(); 
	auto AIForwardIntention = MoveVelocity.GetSafeNormal();
	
	auto ForwardThrow = FVector::DotProduct(TankForward, AIForwardIntention);
	IntendMoveForward(ForwardThrow);

	auto TurnThrow = FVector::CrossProduct(TankForward, AIForwardIntention).Z;
	
	IntendTurnRight(TurnThrow);
}