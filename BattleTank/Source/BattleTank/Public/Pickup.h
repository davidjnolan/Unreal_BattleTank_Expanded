// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTLETANK_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called on actor overlap
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Called when Pickup is Collected - to be overridden in child classes
	virtual void PickupCollection();

	// Adds the static mesh component to the Pickup
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PickupMesh = nullptr;

	// Set Rotation Rate for Pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	FRotator RotationRate = FRotator(0.0f, 90.0f, 0.0f); // Will rotate the Yaw (Z Axis)

	// Speed Multiplier for Pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	float RotationSpeed = 1.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
