// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MenuPawn.generated.h"

class AMainGameMode;

UCLASS()
class RHYTHMCUBE_API AMenuPawn : public APawn
{
	GENERATED_BODY()

protected:
	// Override functions
	virtual void BeginPlay() override;

private:
	// References
	UPROPERTY()
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;

	// Delegate functions
	UFUNCTION()
	void ScreenMotion(bool bNew);
	UFUNCTION()
	void ReadyPlayState(bool bNew);
	UFUNCTION()
	void ExitState();

	// Variables
	FTimerHandle ScreenMotionTimer;
	float ScreenSizeX;
	float ScreenSizeY;
	float MouseX;
	float MouseY;
};
