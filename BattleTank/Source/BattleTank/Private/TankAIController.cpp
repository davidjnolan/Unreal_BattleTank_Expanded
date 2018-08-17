	// Copyright PixelSpawn 2018

#include "TankAIController.h"
#include "BattleTank.h"
#include "Tank.h" // So we can implement OnDeath
#include "TankAimingComponent.h"
#include "Pickup.h"
#include "BattleTankGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "AI/Navigation/NavigationSystem.h"

// Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetPawn();
	if (!ensure(ControlledTank)) { return; }
	UpdateWanderDestination(ControlledTank);

}

// Sets the Pawn controlled by AIController
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

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto ControlledTank = GetPawn();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }

	bDetectedPlayer = IsPlayerDetected(ControlledTank, PlayerTank);
	
	if (bDetectedPlayer) { 		// Move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);
		AimAtPlayer(ControlledTank, PlayerTank);
	}
	else {
		Wander(ControlledTank);
	}

}

// Returns true if player is within DetectionRadius, false if not
bool ATankAIController::IsPlayerDetected(APawn* ControlledTank, APawn* PlayerTank)
{
	FVector DistanceFromPlayerVector = ControlledTank->GetActorLocation() - PlayerTank->GetActorLocation();
	float DistanceFromPlayer = DistanceFromPlayerVector.Size();
	if (DistanceFromPlayer < DetectionRadius) {
		return true;
	}
	else {
		return false;
	}
}

// Aims barrel towards player and fires if FiringState is Locked
void ATankAIController::AimAtPlayer(APawn* ControlledTank, APawn* PlayerTank)
{
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	if (AimingComponent->GetFiringState() == EFiringState::Locked) {
		AimingComponent->Fire();
	}
}

void ATankAIController::Wander(APawn* ControlledTank)
{
	UE_LOG(LogTemp, Warning, TEXT("%s WanderDestination: %s"), *ControlledTank->GetName(), *WanderDestination.ToString());
	MoveToLocation(WanderDestination, WanderAcceptanceRadius);

	auto Distance = FMath::Abs((ControlledTank->GetActorLocation() - WanderDestination).Size());
	UE_LOG(LogTemp, Warning, TEXT("%s Distance: %f"), *ControlledTank->GetName(), Distance);
	if (Distance <= (WanderAcceptanceRadius + 1000)) {
		UpdateWanderDestination(ControlledTank);
	}
}

void ATankAIController::UpdateWanderDestination(APawn* ControlledTank)
{
	WanderDestination = UNavigationSystem::GetRandomReachablePointInRadius(ControlledTank, ControlledTank->GetActorLocation(), 20000.0f);
}

// Delegate - called when tank is destroyed. Spawns pickups and updates the count of AIControllers in the world
void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) { return; }
	auto PossessedTank = GetPawn();
	SpawnPickupsOnDeath(PossessedTank);
	PossessedTank->DetachFromControllerPendingDestroy();
	PossessedTank->Destroy();

	// Update count of the number of AIcontrollers in the world
	auto Gamemode = (ABattleTankGameModeBase*)GetWorld()->GetAuthGameMode();
	Gamemode->FindAllAIControllers();
}

// Wrapper function to call SpawnPickup multiple times - spawns one Health Pickup and one random Projectile Pickup
void ATankAIController::SpawnPickupsOnDeath(APawn* PossessedTank)
{
	if (!ensure(PossessedTank)) { return; }
	if (!ensure(PickupBlueprint_Health)) { return; }

	SpawnPickup(PossessedTank, PickupBlueprint_Health);

	if (!ensure(AvailableProjectilePickups.Num() > 0)) { return; }

	int Pickup = FMath::RandRange(0, AvailableProjectilePickups.Num() - 1);
	SpawnPickup(PossessedTank, AvailableProjectilePickups[Pickup]);
}

// Spawns a pickup actor
void ATankAIController::SpawnPickup(APawn* PossessedTank, TSubclassOf<APickup> PickupToSpawn)
{
	float SpawnOffsetX = FMath::RandRange(PickupMaxSpawnDistance*-1, PickupMaxSpawnDistance);
	float SpawnOffsetY = FMath::RandRange(PickupMaxSpawnDistance*-1, PickupMaxSpawnDistance);
	FVector SpawnOffset = FVector(SpawnOffsetX, SpawnOffsetY, PickupMaxSpawnDistance);
	FVector SpawnLocation = PossessedTank->GetActorLocation() + SpawnOffset;

	GetWorld()->SpawnActor<APickup>(PickupToSpawn, SpawnLocation, PossessedTank->GetActorRotation());
}


