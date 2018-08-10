// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Pickup_Health.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API APickup_Health : public APickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickup_Health();
	
protected:
	// Called when Pickup is Collected - Gives player more health additional health
	virtual void PickupCollection() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	int32 HeathIncrease = 50;
	
};
