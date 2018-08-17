// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h" // Must be the last include

// Forward Declarations
class UTankAimingComponent;

/**
 * Responsible for helping the player aim
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;
	virtual void SetPawn(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

	// Delegate - called when player tank wins
	UFUNCTION()
	void OnPossessedTankDeath();

	// Start the tank moving the barrel so that a shot would hit where the crosshair intersects the world
	void AimTowardCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(FVector& HitLocation) const;

	bool GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const;

	bool GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const;

	// Location of cross hair as normalized percentage of screenspace
	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.33333f;

	// Distance Tank can aim
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000;

protected:
	// Delegate - called when game is won
	UFUNCTION(BlueprintImplementableEvent, Category = "Win")
	void OnGameWin();

	// Called in AimingComponent is found
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingCompRef);

	// Called when tank dies
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void TankDies();

};
