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
	SpawnPickupsOnDeath(PossessedTank);
	PossessedTank->DetachFromControllerPendingDestroy();
	PossessedTank->Destroy();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("%"), AvailableProjectilePickups.Num())

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
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim towards the player
		auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
		AimingComponent->AimAt(PlayerTank->GetActorLocation());

		if (AimingComponent->GetFiringState() == EFiringState::Locked) {
			AimingComponent->Fire();
		}
	} 
}

void ATankAIController::SpawnPickupsOnDeath(APawn* PossessedTank)
{
	if (!ensure(PossessedTank)) { return; }
	if (!ensure(PickupBlueprint_Health)) { return; }

	SpawnPickup(PossessedTank, PickupBlueprint_Health);

	if (!ensure(AvailableProjectilePickups.Num() > 0)) { return; }

	int Pickup = FMath::RandRange(0, AvailableProjectilePickups.Num() - 1);
	SpawnPickup(PossessedTank, AvailableProjectilePickups[Pickup]);
}

void ATankAIController::SpawnPickup(APawn* PossessedTank, TSubclassOf<APickup> PickupToSpawn)
{
	float SpawnOffsetX = FMath::RandRange(PickupMaxSpawnDistance*-1, PickupMaxSpawnDistance);
	float SpawnOffsetY = FMath::RandRange(PickupMaxSpawnDistance*-1, PickupMaxSpawnDistance);
	FVector SpawnOffset = FVector(SpawnOffsetX, SpawnOffsetY, PickupMaxSpawnDistance);
	FVector SpawnLocation = PossessedTank->GetActorLocation() + SpawnOffset;

	GetWorld()->SpawnActor<APickup>(PickupToSpawn, SpawnLocation, PossessedTank->GetActorRotation());
}


