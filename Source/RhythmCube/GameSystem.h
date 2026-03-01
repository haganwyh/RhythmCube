// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameSystem.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UGameSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<AActor>RealTimeActor = nullptr;
	
};
