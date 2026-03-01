// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

class AMainGameMode;

UENUM()
enum class Theme : int32
{
	CyanNeonpink,
	BlueRed,
	GreenRed,
	BlueGold,
	BlueWhite
};

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API UMainGameInstance : public UGameInstance
{
	UMainGameInstance();
	GENERATED_BODY()
	
public:
	int* GetHighestScore();
	void AddHighestScore(int Score);

	void CreateSaveFile();
	void SaveGame();
	void LoadGame();

	float Offset;
	float BGMVolume;
	float SFXVolume;
	float UIVolume;
	float HitsoundVolume;
	int MaxFPS;
	int Money;
	int SelectedSongID;
	int SelectedDifficulty; // 0 = Normal, 1 = Hard, 2 = Extreme
	int Graphics;
	bool bHardcoreMode;
	bool bKeySwap;
	bool bCameraShake;
	bool bTransparentCube;
	bool bNewPlayer;
	bool bHitAssist;
	Theme UsingTheme;
	TArray<Theme> OwnedTheme;
	TMap<int, int> HighestScore;
	//TArray<TArray<TArray<int>>> HighestScore; // 3D array to store highest score for each song

protected:
	// Override functions
	virtual void OnStart() override;

private:
	// References
	TObjectPtr<AMainGameMode> MainGameMode = nullptr;
};
