// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

// Forward Declarations
class UProjectileMovementComponent;
class UParticleSystemComponent;
class URadialForceComponent;


UCLASS()
class BATTLETANK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();


	// Getters
	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual float GetLaunchSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual float GetReloadTimeInSeconds() const;
	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual int32 GetRoundsLeft() const;
	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual float GetProjectileDamage() const;
	UFUNCTION(BlueprintCallable, Category = "Firing")
	virtual float GetDestroyDelay() const;

	void LaunchProjectile(float Speed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float LaunchSpeed = 20000.0;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Firing")
	int32 RoundsLeft = 20;

	UPROPERTY(EditDefaultsOnly, category = "Firing")
	float ProjectileDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, category = "Firing")
	float DestroyDelay = 7.0f;

	UPROPERTY(EditDefaultsOnly, category = "Firing")
	float ForceRadiusDefault = 400.0f;

	UPROPERTY(EditDefaultsOnly, category = "Firing")
	float BlastRadius = 1.0f;


private: 
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void OnTimerExpire();

	UProjectileMovementComponent* ProjectileMovement = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* CollisionMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* LaunchBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* ImpactBlast = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* ExplosionForce = nullptr;
	
};
