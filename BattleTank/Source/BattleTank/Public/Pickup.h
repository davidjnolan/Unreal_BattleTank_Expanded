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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* PickupMesh = nullptr;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
