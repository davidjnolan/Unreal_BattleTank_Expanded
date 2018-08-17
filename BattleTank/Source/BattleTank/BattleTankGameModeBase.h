// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleTankGameModeBase.generated.h"

/**
 * 
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

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Win")
	void GameWin();

public:
	// Finds all of the AI controllers in the world, stores in FoundAIControllers
	void FindAllAIControllers();
};

