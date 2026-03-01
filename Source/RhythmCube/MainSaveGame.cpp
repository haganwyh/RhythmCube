// Fill out your copyright notice in the Description page of Project Settings.


#include "MainSaveGame.h"

UMainSaveGame::UMainSaveGame()
{
	// Player will set this in game settings | On this device: No hit sound = -0.06, Wit hit sound = 0
	Offset = 0.f;
	BGMVolume = 1.f;
	SFXVolume = 1.f;
	UIVolume = 1.f;
	MaxFPS = 1000;
	HitsoundVolume = 1.f;
	Money = 30000;
	SelectedSongID = 3;
	SelectedDifficulty = 0;
	Graphics = 0;
	bHardcoreMode = false;
	bKeySwap = false;
	bCameraShake = true;
	bTransparentCube = true;
	bNewPlayer = true;
	bHitAssist = true;
	UsingTheme = Theme::BlueRed;
	OwnedTheme = { Theme::BlueRed };
}