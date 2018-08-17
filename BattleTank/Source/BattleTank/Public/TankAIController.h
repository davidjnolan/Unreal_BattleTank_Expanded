// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

// Forward Declarations
class APickup;

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnPossessedTankDeath();

	// Wrapper function to call SpawnPickup multiple times
	void SpawnPickupsOnDeath(APawn* PossessedTank);
	// Spawns a pickup actor
	void SpawnPickup(APawn* PossessedTank, TSubclassOf<APickup> PickupToSpawn);

protected:
	// How close can the AI tank get to the player
	UPROPERTY(EditDefaultsOnly, Category = "Setup") 
	float AcceptanceRadius = 7000;

	// How close can the player get to AI tank before the AI notices her
	UPROPERTY (EditDefaultsOnly, Category = "Setup")
	float DetectionRadius = 25000;

	// Maximum distance that a pickup will spawn from destroyed Tank
	UPROPERTY (EditDefaultsOnly, Category = "Setup")
	float PickupMaxSpawnDistance = 300.0f;

	// Array of the Sets the Health Pickup Blueprint
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<APickup> PickupBlueprint_Health;

	// Array of the Projectile Pickups which can be dropped
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TArray<TSubclassOf<APickup>> AvailableProjectilePickups;

	// Tracks whether the player has been detected by the AI controller
	bool bDetectedPlayer = false;


};
