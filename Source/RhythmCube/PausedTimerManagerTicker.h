// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PausedTimerManagerTicker.generated.h"

class AMainGameMode;

UCLASS()
class APausedTimerManagerTicker : public AActor
{
	GENERATED_BODY()

	// Constructor
	APausedTimerManagerTicker();

	// Override functions
	virtual void Tick(float DeltaSeconds) override;

protected:
	// Override functions
	virtual void BeginPlay() override;
};