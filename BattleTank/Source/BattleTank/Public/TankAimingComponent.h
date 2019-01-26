// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum For Aming Colors
UENUM()
enum class EFiringState : uint8 {
	Reloading,
	Amiming,
	Locked,
	OutOfAmmo
};

class UTankBarrel;
class UTankTurret;
class AProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

private:	
	// Sets default values for this component's properties
	UTankAimingComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void BeginPlay() override;
protected:
	UPROPERTY(BlueprintReadonly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

	bool IsBarrelMoving();

public:	
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	EFiringState GetFiringState() const;

	void AimAt(FVector HitLocation);



	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;



	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 1000000; // Sensible starting value of 1000m/s

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	double LastFireTime = 0;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	int GetRoundsLeft() const;
private:

	void MoveBarrelToward(FVector AimDirection);

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	FVector AimDirection;

	int RoundsLeft = 3;

};
