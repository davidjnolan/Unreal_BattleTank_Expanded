// Copyright PixelSpawn 2018

#include "TankPlayerController.h"
#include "Tank.h" // So we can implement OnDeath
#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "BattleTankGameModeBase.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"



void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!GetPawn()) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return;	}
	FoundAimingComponent(AimingComponent);

	//Gamemode = (ABattleTankGameModeBase*)GetWorld()->GetAuthGameMode(); // Find the Gamemode
}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe out local method to the tank's death event
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);

		// Subscribe to Gamemode win event delegate
		auto Gamemode = (ABattleTankGameModeBase*)GetWorld()->GetAuthGameMode();
		if (!ensure(Gamemode)) { return; }
		Gamemode->OnWin.AddUniqueDynamic(this, &ATankPlayerController::OnGameWin);
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetPawn()) { return; }
	AimTowardCrosshair();
}

// Delegate - called when player tank wins
void ATankPlayerController::OnPossessedTankDeath()
{
	APlayerController::StartSpectatingOnly();
	TankDies();
}

// Start the tank moving the barrel so that a shot would hit where the crosshair intersects the world
void ATankPlayerController::AimTowardCrosshair()
{
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation; // Out Parameter
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
	if (bGotHitLocation) // Has "side-effect", is going to line trace
	{
		AimingComponent->AimAt(HitLocation);
	}
}

// Get world location if linetrace through crosshair, true if hits landscape. Returns HitLocation as an OUT parameter
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	// Ray cast through camera to geometry
	//set to HitLocation
	HitLocation = FVector(1.0f);

	// Find the crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

	// "De-project" the screen position of the crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		// Line-trace along that LookDirection and see what we hit (up to max range)
		return GetLookVectorHitLocation(LookDirection, HitLocation);
	}
	return false;
}

// Deproject screen position into world to get look direction, true if deprojection successful. Returns LookDirection as an OUT parameter
bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; // To be discarded
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection
	);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);
	if (GetWorld()->LineTraceSingleByChannel( // if LineTrace succeeds
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Camera
	)) {
		HitLocation = HitResult.Location; // set HitLocation
		return true;
	}
	else { // Line Trace failed
		HitLocation = FVector(0);
		return false; 
	}
}

