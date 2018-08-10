// Copyright PixelSpawn 2018

#include "Pickup_Projectile.h"

APickup_Projectile::APickup_Projectile()
{

}

void APickup_Projectile::PickupCollection()
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Pickup Collected!"));
	ChangeProjectile();
}


