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
	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void Fire();

	EFiringState GetFiringState() const;
	
	UFUNCTION(BlueprintCallable, Category = "Firing")
	int32 GetRoundsLeft() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Firing")
	TSubclassOf<AProjectile> GetProjectileBP() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void SetProjectileBP(TSubclassOf<AProjectile> NewProjectileBlueprint);

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringState FiringState = EFiringState::Reloading;

	UFUNCTION(BlueprintCallable, Category = "Firing")
	void SetRoundsLeft(int32 NewRoundsCount);

	// Gets the LaunchSpeed, ReloadTimeInSeconds and RoundsLeft defaults from referenced projectile and sets them in the aiming component
	UFUNCTION(BlueprintCallable, Category = "Firing")
	void ApplyProjectileSettings();

private:
	// Sets default values for this component's properties
	UTankAimingComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	void MoveBarrelTowards(FVector AimDirection);

	bool IsBarrelMoving();

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(VisibleAnywhere, Category = "Firing")
	float LaunchSpeed = 20000.0;

	UPROPERTY(VisibleAnywhere, Category = "Firing")
	float ReloadTimeInSeconds = 2.0f;

	UPROPERTY(VisibleAnywhere, Category = "Firing")
	int32 RoundsLeft = 5;

	double LastFireTime = 0;

	FVector AimDirection;

	AProjectile* Projectile = nullptr;


};
