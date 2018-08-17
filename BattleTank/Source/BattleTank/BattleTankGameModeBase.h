// Copyright PixelSpawn 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleTankGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameModeDelegate);

/**
 * Game mode - determines the win state for the game
 */
UCLASS()
class BATTLETANK_API ABattleTankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// Keeps track of all AI Controllers in the world
	TArray<AActor*> FoundAIControllers;

public:
	// Finds all of the AI controllers in the world, stores in FoundAIControllers
	void FindAllAIControllers();

	// Delegate
	FGameModeDelegate OnWin;
};

