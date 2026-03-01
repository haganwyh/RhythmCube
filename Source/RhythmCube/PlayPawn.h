// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayPawn.generated.h"

class AMainGameMode;

UCLASS()
class RHYTHMCUBE_API APlayPawn : public APawn
{
	GENERATED_BODY()

public:
	void CameraShake(float Scale);

private:
	// Camera shake
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CamShake;
};
