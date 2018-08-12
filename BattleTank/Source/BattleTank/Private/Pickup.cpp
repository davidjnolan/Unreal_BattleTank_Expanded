// Copyright PixelSpawn 2018

#include "Pickup.h"

#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Engine/World.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Components
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("PickupMesh"));
	SetRootComponent(PickupMesh);
	PickupMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	PickupMesh->SetCastShadow(false);

	PickupLight = CreateDefaultSubobject<UPointLightComponent>(FName("PickupLight"));
	PickupLight->AttachTo(PickupMesh);
	PickupLight->SetIntensity(20000.0f);

	
	

		
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	PickupMesh->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlap);
	PickupLight->SetLightColor(PickupColor);
}

// Returns the PickupColor
FLinearColor APickup::GetPickupColor() const { return PickupColor; }


void APickup::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto PlayerTank = Cast<AActor>(GetWorld()->GetFirstPlayerController()->GetPawn()); // Get PlayerTank
	if (!PlayerTank) { return; }
	if (OtherActor == PlayerTank){ 
		PickupCollection();
		Destroy();
	}
	else
	{
		return; // Return if overlapped actor is not player tank
	}
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Apply rotation to Pickup
	AddActorLocalRotation(RotationRate * DeltaTime * RotationSpeed);
}


void APickup::PickupCollection()
{
	UE_LOG(LogTemp, Warning, TEXT("Collected!"));
}


