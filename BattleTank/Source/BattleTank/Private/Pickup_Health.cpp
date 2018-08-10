// Copyright PixelSpawn 2018

#include "Pickup_Health.h"
#include "Tank.h"


// Sets default values

APickup_Health::APickup_Health()
{

}

void APickup_Health::PickupCollection()
{
	ATank* PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn()); // Get PlayerTank

	int32 CurrentHealth = PlayerTank->GetHealth();
	int32 MaxHealth = PlayerTank->GetStartingHealth();

	int32 NewHealth = CurrentHealth + HeathIncrease;
	NewHealth = FMath::Clamp(NewHealth, 0, MaxHealth);

	PlayerTank->SetHealth(NewHealth);

}



