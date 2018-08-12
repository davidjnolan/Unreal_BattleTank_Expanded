// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Enum for aiming state
UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

// Forward Declaration
class UTankBarrel;
class UTankTurret;
class AProjectile;

// Holds barrel's properties and Elevate method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Initialize the Tank Barrel and Turret Variables in BluePrint
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	// Determines how many sockets the attached Barrel has
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void FindBarrelSockets(TArray<FName> FoundBarrelSockets, FName FirstBarrelSocket, int32 FoundNumBarrelSockets);

	// Takes in the desired HitLocation, calculates the AimDirection and calls MoveBarrelTowards()
	void AimAt(FVector HitLocation);

	// Fire the projectile
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	// Returns the current Firing State ENUM
	EFiringState GetFiringState() const;
	
protected:
	// Returns the currently referenced Projectile Blueprint
	UFUNCTION(BlueprintCallable, Category = "Firing")
	TSubclassOf<AProjectile> GetProjectileBP() const;

	// Sets a new Projectile Blueprint to be referenced
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void SetProjectileBP(TSubclassOf<AProjectile> NewProjectileBlueprint);

	// Returns the number of remaining rounds
	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetRoundsLeft() const;

	// Sets a new value for the remaining rounds
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void SetRoundsLeft(int32 NewRoundsCount);

	// Gets the LaunchSpeed, ReloadTimeInSeconds and RoundsLeft defaults from referenced projectile and sets them in the aiming component
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void ApplyProjectileSettings();

	// Variable for the current firing state
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// Moves the barrel towards the AimDirection
	void MoveBarrelTowards(FVector AimDirection);

	// Returns true if barrel is moving, otherwise false - used in Tick to determine the current firing state
	bool IsBarrelMoving();



	// Updates the variables to determine which barrel fires next
	void UpdateBarrelSockets();

	// Checks if tank has rounds left, whether tank is reloaded, and if the barrel is moving to set the firing state Enum
	void DetermineFiringState();

	// Sets null pointers for the Barrel, Turret and Projectile - to be initialized elsewhere
	UTankBarrel* Barrel = nullptr;     // set in the Initialize() method
	UTankTurret* Turret = nullptr;     // set in the Initialize() method
	AProjectile* Projectile = nullptr; // set when the projectile is spawned

	// Creates the Projectile Blueprint Reference - set in the TankAimingComponent Blueprint UProperty
	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	// Variables for firing calculations - initialized here with basic default values. These are set with the values from the Projectile Blueprint using ApplyProjectileSettings()
	UPROPERTY(VisibleAnywhere, Category = "Firing")
	float LaunchSpeed = 20000.0;
	UPROPERTY(VisibleAnywhere, Category = "Firing")
	int32 RoundsLeft = 5;
	UPROPERTY(VisibleAnywhere, Category = "Firing")
	float ReloadTimeInSeconds = 2.0f;
	UPROPERTY(VisibleAnywhere, Category = "Firing")
	float MultiBarrelReloadTimeInSeconds = 0.15f;

	float OriginalReloadTimeInSeconds = ReloadTimeInSeconds;

	// Stores time since tank last fired
	double LastFireTime = 0;

	// Stores the AimDirection
	FVector AimDirection;
	
	// Stores the BarrelSockets
	TArray<FName> BarrelSockets;
	FName CurrentBarrelSocket;
	int32 NumBarrelSockets = 1;
	int32 LastFiredBarrelIndex = 0;
	


};
