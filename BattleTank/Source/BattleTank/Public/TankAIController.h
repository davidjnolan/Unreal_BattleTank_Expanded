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

	void SpawnPickups(APawn* PossessedTank);

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<APickup> PickupBlueprint_Health;

protected:
	// How close can the AI tank get to the player
	UPROPERTY(EditDefaultsOnly, Category = "Setup") 
	float AcceptanceRadius = 7000;

	UPROPERTY (EditDefaultsOnly, Category = "Setup")
	float DetectionRadius = 25000;

	UPROPERTY (EditDefaultsOnly, Category = "Setup")
	float PickupMaxSpawnDistance = 300.0f;

	bool bDetectedPlayer = false;


};
