// Copyright PixelSpawn 2018

#include "BattleTankGameModeBase.h"
#include "TankAIController.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


void ABattleTankGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FindAllAIControllers();
}

void ABattleTankGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ABattleTankGameModeBase::FindAllAIControllers()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATankAIController::StaticClass(), FoundAIControllers);
	if (FoundAIControllers.Num() <= 0) {
		OnWin.Broadcast();
	}
}
