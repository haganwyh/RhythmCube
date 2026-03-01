// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameInstance.h"
#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MainSaveGame.h"



UMainGameInstance::UMainGameInstance()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	// Player will set this in game settings | On this device: No hit sound = -0.06, Wit hit sound = 0
	Offset = 0.f;
	BGMVolume = 1.f;
	SFXVolume = 1.f;
	UIVolume = 1.f;
	MaxFPS = 1000;
	HitsoundVolume = 1.f;
	Money = 0;
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
	/*
	for (int i = 0; i < 3; i++) // 3 = Max number of song, increase when new song is added
	{
		HighestScore.Add({ {NULL, NULL}, {NULL, NULL}, {NULL, NULL} });
	}
	*/
}

void UMainGameInstance::OnStart()
{
	Super::OnStart();
	LoadGame();
}

int* UMainGameInstance::GetHighestScore()
{
	/*
	Work like a 3D array:
	Song0: 0-5   NORMAL0 NORMAL1 HARD0 HARD1 EXTREME0 EXTREME1
	Song1: 6-11  NORMAL0 NORMAL1 HARD0 HARD1 EXTREME0 EXTREME1
	Song2: 12-17 NORMAL0 NORMAL1 HARD0 HARD1 EXTREME0 EXTREME1
	*/
	return HighestScore.Find(6 * SelectedSongID + 2 * SelectedDifficulty + bHardcoreMode);
}

void UMainGameInstance::AddHighestScore(int Score)
{
	HighestScore.Add(6 * SelectedSongID + 2 * SelectedDifficulty + bHardcoreMode, Score);
}

void UMainGameInstance::CreateSaveFile()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating"));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Creating"))));
	UMainSaveGame* DataToSave = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));
	UGameplayStatics::SaveGameToSlot(DataToSave, "SlotDefault", 0);
	UE_LOG(LogTemp, Warning, TEXT("Created"));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Created"))));
}

void UMainGameInstance::SaveGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Saving"));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Saving"))));
	UMainSaveGame* DataToSave = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot("SlotDefault", 0));
	if (DataToSave != nullptr)
	{
		// Data to save
		DataToSave->Offset = Offset;
		DataToSave->BGMVolume = BGMVolume;
		DataToSave->SFXVolume = SFXVolume;
		DataToSave->UIVolume = UIVolume;
		DataToSave->HitsoundVolume = HitsoundVolume;
		DataToSave->MaxFPS = MaxFPS;
		DataToSave->Money = Money;
		DataToSave->SelectedSongID = SelectedSongID;
		DataToSave->SelectedDifficulty = SelectedDifficulty;
		DataToSave->Graphics = Graphics;
		DataToSave->bHardcoreMode = bHardcoreMode;
		DataToSave->bKeySwap = bKeySwap;
		DataToSave->bCameraShake = bCameraShake;
		DataToSave->bTransparentCube = bTransparentCube;
		DataToSave->bHitAssist = bHitAssist;
		DataToSave->bNewPlayer = bNewPlayer;
		DataToSave->UsingTheme = UsingTheme;
		DataToSave->OwnedTheme = OwnedTheme;
		DataToSave->HighestScore = HighestScore;
		UGameplayStatics::SaveGameToSlot(DataToSave, "SlotDefault", 0);
		UE_LOG(LogTemp, Warning, TEXT("Saved"));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Saved"))));
	}
	else if (!UGameplayStatics::DoesSaveGameExist("SlotDefault", 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Fail"))));
		CreateSaveFile();
	}
}

void UMainGameInstance::LoadGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading"));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Loading"))));
	UMainSaveGame* DataToLoad = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot("SlotDefault", 0));
	if (DataToLoad != nullptr)
	{
		Offset = DataToLoad->Offset;
		BGMVolume = DataToLoad->BGMVolume;
		SFXVolume = DataToLoad->SFXVolume;
		UIVolume = DataToLoad->UIVolume;
		HitsoundVolume = DataToLoad->HitsoundVolume;
		MaxFPS = DataToLoad->MaxFPS;
		Money = DataToLoad->Money;
		SelectedSongID = DataToLoad->SelectedSongID;
		SelectedDifficulty = DataToLoad->SelectedDifficulty;
		Graphics = DataToLoad->Graphics;
		bHardcoreMode = DataToLoad->bHardcoreMode;
		bKeySwap = DataToLoad->bKeySwap;
		bCameraShake = DataToLoad->bCameraShake;
		bTransparentCube = DataToLoad->bTransparentCube;
		bHitAssist = DataToLoad->bHitAssist;
		bNewPlayer = DataToLoad->bNewPlayer;
		UsingTheme = DataToLoad->UsingTheme;
		OwnedTheme = DataToLoad->OwnedTheme;
		HighestScore = DataToLoad->HighestScore;
		UE_LOG(LogTemp, Warning, TEXT("Loaded"));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Loaded"))));
	}
	else if (!UGameplayStatics::DoesSaveGameExist("SlotDefault", 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Fail"))));
		CreateSaveFile();
	}
}