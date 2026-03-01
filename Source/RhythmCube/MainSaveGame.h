// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MainGameInstance.h"
#include "MainSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UMainSaveGame : public USaveGame
{
	UMainSaveGame();
	GENERATED_BODY()
	
public:
	UPROPERTY()
	float Offset;
	UPROPERTY()
	float BGMVolume;
	UPROPERTY()
	float SFXVolume;
	UPROPERTY()
	float UIVolume;
	UPROPERTY()
	float HitsoundVolume;
	UPROPERTY()
	int MaxFPS;
	UPROPERTY()
	int Money;
	UPROPERTY()
	int SelectedSongID;
	UPROPERTY()
	int SelectedDifficulty;
	UPROPERTY()
	int Graphics;
	UPROPERTY()
	bool bHardcoreMode;
	UPROPERTY()
	bool bKeySwap;
	UPROPERTY()
	bool bCameraShake;
	UPROPERTY()
	bool bTransparentCube;
	UPROPERTY()
	bool bNewPlayer;
	UPROPERTY()
	bool bHitAssist;
	UPROPERTY()
	Theme UsingTheme;
	UPROPERTY()
	TArray<Theme> OwnedTheme;
	UPROPERTY()
	TMap<int, int> HighestScore;
};
