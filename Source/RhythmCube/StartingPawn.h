// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StartingPawn.generated.h"

class AMainGameMode;

UCLASS()
class RHYTHMCUBE_API AStartingPawn : public APawn
{
	GENERATED_BODY()

public:
	void CameraShake(float Scale);

protected:
	// Override functions
	virtual void BeginPlay() override;

private:
	// Camera shake
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> CamShake;

	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;

	// Delegate functions
	UFUNCTION()
	void ScreenMotion();
	UFUNCTION()
	void ReadyMainMenu();

	// Variables
	FTimerHandle ScreenMotionTimer;
	float ScreenSizeX;
	float ScreenSizeY;
	float MouseX;
	float MouseY;
};
