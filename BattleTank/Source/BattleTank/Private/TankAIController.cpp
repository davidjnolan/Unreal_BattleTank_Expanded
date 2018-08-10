	// Copyright PixelSpawn 2018

#include "TankAIController.h"
#include "BattleTank.h"
#include "Tank.h" // So we can implement OnDeath
#include "TankAimingComponent.h"
#include "Pickup.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe out local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) { return; }
	auto PossessedTank = GetPawn();
	SpawnPickups(PossessedTank);
	PossessedTank->DetachFromControllerPendingDestroy();
	PossessedTank->Destroy();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetPawn()) { return; }
	if (!GetWorld()->GetFirstPlayerController()->GetPawn()) { return; }

	auto ControlledTank = GetPawn();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }

	FVector DistanceFromPlayerVector = ControlledTank->GetActorLocation() - PlayerTank->GetActorLocation();
	float DistanceFromPlayer = DistanceFromPlayerVector.Size();
	if (DistanceFromPlayer < DetectionRadius) {
		bDetectedPlayer = true;
	}
	else {
		bDetectedPlayer = false;
	}	

	if (bDetectedPlayer) {
		// Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius); // TODO check radius is in cm

		// Aim towards the player
		auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		if (AimingComponent->GetFiringState() == EFiringState::Locked) {
			AimingComponent->Fire(); // TODO limit firing rate
		}
	} 
}

void ATankAIController::SpawnPickups(APawn* PossessedTank)
{
	if (!ensure(PossessedTank)) { return; }
	if (!ensure(PickupBlueprint_Health)) { return; }

	float SpawnOffsetX = FMath::RandRange(PickupMaxSpawnDistance*-1, PickupMaxSpawnDistance);
	float SpawnOffsetY = FMath::RandRange(PickupMaxSpawnDistance*-1, PickupMaxSpawnDistance);
	FVector SpawnOffset = FVector(SpawnOffsetX, SpawnOffsetY, PickupMaxSpawnDistance);
	FVector SpawnLocation = PossessedTank->GetActorLocation() + SpawnOffset;

	GetWorld()->SpawnActor<APickup>(PickupBlueprint_Health, SpawnLocation, PossessedTank->GetActorRotation());
}
