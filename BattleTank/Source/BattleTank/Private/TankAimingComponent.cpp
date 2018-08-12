// Copyright PixelSpawn 2018

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Public/TimerManager.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	ApplyProjectileSettings();
	//FindBarrelSockets();
	LastFireTime = GetWorld()->GetTimeSeconds(); 	// So that first fire is after inital reload
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DetermineFiringState();
}

// Returns the current Firing State ENUM
EFiringState UTankAimingComponent::GetFiringState() const {	return FiringState; }

// Returns the currently referenced Projectile Blueprint
TSubclassOf<AProjectile> UTankAimingComponent::GetProjectileBP() const { return ProjectileBlueprint; }

// Sets a new Projectile Blueprint to be referenced
void UTankAimingComponent::SetProjectileBP(TSubclassOf<AProjectile> NewProjectileBlueprint) { ProjectileBlueprint = NewProjectileBlueprint; }

// Returns the number of remaining rounds
int32 UTankAimingComponent::GetRoundsLeft() const {	return RoundsLeft; }

// Sets a new value for the remaining rounds
void UTankAimingComponent::SetRoundsLeft(int32 NewRoundsCount) { RoundsLeft = NewRoundsCount; }

// Initialize the Tank Barrel and Turret Variables in BluePrint
void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	if (!ensure(BarrelToSet && TurretToSet)) { return; }
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Gets the LaunchSpeed, ReloadTimeInSeconds and RoundsLeft defaults from referenced projectile and sets them in the aiming component
void UTankAimingComponent::ApplyProjectileSettings()
{
	if (!ensure(ProjectileBlueprint)) { return; }
	FTransform SpawnLocation = FTransform(FVector(0.0f, 0.0f, -5000.0f));
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, SpawnLocation);

	LaunchSpeed = Projectile->GetLaunchSpeed();
	ReloadTimeInSeconds = Projectile->GetReloadTimeInSeconds();
	OriginalReloadTimeInSeconds = Projectile->GetReloadTimeInSeconds();
	RoundsLeft = Projectile->GetRoundsLeft();
	Projectile->Destroy();
}

// Takes in the desired HitLocation, calculates the AimDirection and calls MoveBarrelTowards()
void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(CurrentBarrelSocket);
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity // Calculate the OutLaunchVelocity
	( 	
		this,
		OutLaunchVelocity, // Out parameter
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false, // will favour low arc
		0.0f, // CollisionRadius,
		0.0f, // Gravity Override
		ESuggestProjVelocityTraceOption::DoNotTrace,
		FCollisionResponseParams::DefaultResponseParam,
		TArray < AActor * >(), // actors to ignore - empty arry
		false // draw debug
	);
	if (bHaveAimSolution) {
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
	}
	else { // If no solution, do nothing
	}
}

// Fire the projectile
void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming) { 
		// Spawn a projectile at the socket location of the barrel
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }
		
		Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint,
														 Barrel->GetSocketLocation(CurrentBarrelSocket),
														 Barrel->GetSocketRotation(CurrentBarrelSocket)
														 );
		Projectile->LaunchProjectile(LaunchSpeed);
		UpdateBarrelSockets();
		LastFireTime = GetWorld()->GetTimeSeconds();
		RoundsLeft--;
	}
	// TODO - create a class level TArray to store the SocketNames, and a function to iterate through
}

// Moves the barrel towards the AimDirection
void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }

	// Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	// Always Yaw the shortest way
	if (FMath::Abs(DeltaRotator.Yaw) < 180) {
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else { // Avoid going the long way round
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

// Returns true if barrel is moving, otherwise false
bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.01f);
}

// Determines how many sockets the attached Barrel has
void UTankAimingComponent::FindBarrelSockets(TArray<FName> FoundBarrelSockets, FName FirstBarrelSocket, int32 FoundNumBarrelSockets)
{
	if (!ensure(Barrel)) { return; }
	BarrelSockets = FoundBarrelSockets;
	CurrentBarrelSocket = FirstBarrelSocket;
	NumBarrelSockets = FoundNumBarrelSockets;
}

// Updates the variables to determine which barrel fires next
void UTankAimingComponent::UpdateBarrelSockets() {
	LastFiredBarrelIndex += 1;
	if (LastFiredBarrelIndex >= NumBarrelSockets) {
		LastFiredBarrelIndex = 0;
		ReloadTimeInSeconds = OriginalReloadTimeInSeconds;
	}
	else {
		ReloadTimeInSeconds = MultiBarrelReloadTimeInSeconds;
	}
	CurrentBarrelSocket = BarrelSockets[LastFiredBarrelIndex];
}

// Checks if tank has rounds left, whether tank is reloaded, and if the barrel is moving to set the firing state Enum
void UTankAimingComponent::DetermineFiringState()
{
	if (RoundsLeft <= 0) {
		FiringState = EFiringState::OutOfAmmo;
	}
	else if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeInSeconds) {
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving()) {
		FiringState = EFiringState::Aiming;
	}
	else {
		FiringState = EFiringState::Locked;
	}
}
