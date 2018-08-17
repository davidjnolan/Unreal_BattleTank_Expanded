// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h" // Put new includes above

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);


/*
* Base class for all tanks
*/


UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

private:
	// Sets default values for this pawn's properties
	ATank();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth = StartingHealth;

public:
	// Called by the engine when actor damage is dealt
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Returns CurrentHealth
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(int32 NewHealth);

	// Returns StartingHealth
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetStartingHealth() const;

	// Returns CurrentHealth as a percentage of Starting Health between 0-1
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthPercent() const;
	
	FTankDelegate OnDeath;



};
