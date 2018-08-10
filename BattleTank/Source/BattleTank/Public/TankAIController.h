// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

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

protected:
	// How close can the AI tank get to the player
	UPROPERTY(EditDefaultsOnly, Category = "Setup") 
	float AcceptanceRadius = 7000;

	UPROPERTY (EditDefaultsOnly, Category = "Setup")
	float DetectionRadius = 25000;

	bool bDetectedPlayer = false;


};
