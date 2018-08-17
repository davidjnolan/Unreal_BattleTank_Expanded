// Fill out your copyright notice in the Description page of Project Settings.

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
	UE_LOG(LogTemp, Warning, TEXT("Found AI Controllers: %i"), FoundAIControllers.Num());
}

void ABattleTankGameModeBase::FindAllAIControllers()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATankAIController::StaticClass(), FoundAIControllers);
	if (FoundAIControllers.Num() <= 0) {
		GameWin();
	}
}
