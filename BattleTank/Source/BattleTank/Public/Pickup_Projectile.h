// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Pickup_Projectile.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API APickup_Projectile : public APickup
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APickup_Projectile();
	
protected:
	// Called when Pickup is Collected - Gives player more health additional health
	virtual void PickupCollection() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void ChangeProjectile();
	
};
