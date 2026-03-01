// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameInstance.h"
#include "MainGameMode.generated.h"

class UAudioComponent;
class AMainPlayerController;
class UMainGameInstance;
class ACubeBase;
class ATrackLightsBase;
class USoundWave;
class AMainHUD;
class USpotLightComponent;
class UMaterialParameterCollection;
class UGameUserSettings;
class AAmbientLightBarBase;

// Declare enum
UENUM()
enum class BGM : int32
{
	Song0,
	Song1,
	Song2,
	Song3,
	PreSong0,
	PreSong1,
	PreSong2,
	PreSong3,
	SongTutorial,
	GameOverState,
	GameOverFullScore,
	SettingState,
	StoreState,
	StartingMenu
};

UENUM()
enum class SoundEffects : int32
{
	Blue,
	Red,
	Boom,
	Click,
	Miss,
	SeeYouSoon,
	Positive,
	Warning,
	StartClick
};

UENUM()
enum class Grade : int32
{
	SSS, // SSS = S+
	SS, // SS = S
	S, // S = S-
	AAA,
	AA,
	A,
	BBB,
	BB,
	B,
	CCC,
	CC,
	C,
	FF
};

UENUM()
enum class Accuracy : int32
{
	Perfect,
	Good,
	Early,
	Miss
};

// Declare delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStates);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitResultAccuracy, Accuracy, Result, int, ID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegateTwoParams, int, ID, FString, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameStatesOneParam, bool, bNew);

/**
 * 
 */
UCLASS()
class RHYTHMCUBE_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Create delegate
	FGameStatesOneParam OnMainMenu;
	FGameStatesOneParam OnReadyPlayState;
	FHitResultAccuracy OnHitResult;
	FDelegateTwoParams OnUpdateSelectedSong;
	FDelegateTwoParams OnUpdateSongInfo;
	FGameStates OnPlayStart;
	FGameStates OnGameOverState;
	FGameStates OnPauseState;
	FGameStates OnUnPauseState;
	FGameStates OnExitState;
	FGameStates OnPlayerDie;
	FGameStates OnSettingState;
	FGameStates OnStoreState;
	FGameStates OnStartState;
	FGameStates OnReadyMainMenu;
	FGameStates OnLoadGame;
	FDelegate OnHealthChanged;
	FDelegate OnBuySuccess;
	
	// Variables
	static constexpr float CubeEndX = -3640.f;
	static constexpr float Deviation = 0.2f;
	static constexpr float PerfectDeviation = 0.1f;
	TArray<TArray<FLinearColor>> ThemeColors = {
		{FLinearColor(0, 1, 1, 1) * 0.4f, FLinearColor(0.913099f, 0.035601f, 0.401978f, 1)}, // Cyan and neon pink
		{FLinearColor(0, 0, 1, 1), FLinearColor(1, 0, 0, 1)}, // Blue and red
		{FLinearColor(0, 1, 0, 1) * 0.4f, FLinearColor(1, 0, 0, 1)}, // Green and red
		{FLinearColor(0, 0, 1, 1), FLinearColor(0.693f, 0.222978f, 0.093844f, 1)}, // Blue and gold
		{FLinearColor(0, 0, 1, 1), FLinearColor(1, 1, 1, 1) * 0.4f} // Blue and white
	};
	TArray<int> ThemePrices = {
		10000,
		0,
		5000,
		5000,
		3000
	};
	TArray<FString> SongNames = {
		"Madison Beer - Good In Goodbye",
		"Vicetone - Nevada (feat. Cozi Zuehlsdorff)",
		"Verse One - Gimme The Light",
		"Chronomia / Lime (BGA)"
	};
	TArray<TArray<FString>> SongCredit = {
		{"Badison Beer", "Yampa"},
		{"Vicetone", "Francis Drake"},
		{"Verse One", "Akama Zenta"},
		{"Lime", "inukora"}
	};
	TArray<int> DisableSongsFullID = {0, 2, 3, 4, 8, 10}; // FullID = SongID * 3 + Difficulty level
	TArray<float> TutorialTimeCode = {
		// Introduction (0-5)
		8.f,
		8.f,
		8.f,
		8.f,
		16.f,
		8.f,
		// Blue cubes (6-11)
		8.f,
		24.f,
		16.f,
		16.f,
		16.f,
		40.f,
		// Red cubes (12-17)
		8.f,
		20.f,
		16.f,
		16.f,
		16.f,
		60.f,
	};
	float CubeSpawnX;
	float BpmTime; // Time between 1 beat
	float NextBeatTime;
	float Score;
	int Combo;
	int MaxCombo;
	int HitSuccessCount;
	int MoneyGain;
	int PreBeat;
	int Health;
	bool bEnablePause; // Actual enable or disable pause function
	bool bGameStateEnablePause; // Manage the pause permission of entire game state
	bool bHighestScore;
	bool bManagedPlayerDieEvent;
	bool bTutorial;
	Grade PlayerGrade;
	TArray<TArray<TObjectPtr<ACubeBase>>> GeneratedCubes = { {}, {}, {}, {} }; // 2D array to store generated cubes, in order

	// Functions
	void PlayBGM(BGM Song, float FadeOut, float FadeIn);
	void StopBGM(float FadeOut);
	void PauseBGM();
	void ResumeBGM();
	void PlaySoundEffects(SoundEffects Audio);
	void ResetCombo();
	void GainScore(Accuracy HitAccuracy);
	void DeregisterCube(ACubeBase* Target, int ID);
	void ReadyPlayState(bool bNew);
	void PauseState();
	void UnPauseState();
	void MainMenu(bool bNew);
	void ExitState();
	void UpdateSelectedSong();
	void Hurt();
	void SettingState();
	void StoreState();
	void UpdateColorTheme();
	void UpdateColorTheme(Theme ColorTheme);
	void UpdateColorThemeWithAnimation();
	void StartState();
	void ReadyMainMenu();
	void UpdateGraphics();
	void UpdateFPS();
	void UpdateBGMVolume();
	void UpdateSFXVolume();
	void UpdateUIVolume();
	void UpdateHitsoundVolume();
	void BenchmarkGraphicsInitialize();
	void GameOverState();
	void ShowTrackLights(int ID, int mode);
	Grade GetPlayerGrade(int CurrentScore);

protected:
	// Override functions
	virtual void BeginPlay() override;

private:
	// Exposed references
	UPROPERTY(EditAnywhere, Category = "Audios")
	TArray<TObjectPtr<USoundWave>> Audios;
	UPROPERTY(EditAnywhere, Category = "Audios")
	TArray<TObjectPtr<USoundWave>> SoundEffectAudios;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> ActorsToSpawn;
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<ATrackLightsBase>> TrackLights;
	UPROPERTY(EditAnywhere)
	TArray<TSoftObjectPtr<AAmbientLightBarBase>> AmbientLightBars;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UMaterialInterface>> MaterialsToForceLoad;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialParameterCollection> ColorThemeParameterCollection = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundMix> GameSoundMix = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundClass> BGMSoundClass = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundClass> HitSoundClass = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundClass> SFXSoundClass = nullptr;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundClass> UISoundClass = nullptr;

	// Private references
	UPROPERTY()
	TArray<TObjectPtr<UAudioComponent>> AudioComponents;
	UPROPERTY()
	TObjectPtr<AMainPlayerController> MainPlayerController = nullptr;
	UPROPERTY()
	TObjectPtr<AMainHUD> MainHUD = nullptr;
	UPROPERTY()
	TObjectPtr<UMainGameInstance> MainGameInstance = nullptr;
	UPROPERTY()
	TObjectPtr<UGameUserSettings> GameUserSetting = nullptr;
	UPROPERTY()
	TObjectPtr<USpotLightComponent> SpawnSpotLight = nullptr;

	// Variables
	float Bpm;
	float PreDelay;
	float LightsOpacity = 0.f;
	float LevelConstant; // A constant for each level, calculated with specific formula, used to add score with combo
	float NextTutorialTime;
	float LightIntensity;
	float TrackLightsCurrentYaw;
	int GeneratedCount;
	int TutorialCount;
	int Speed; // 1 = 1 BpmTime to approach player, 4 = 4 BpmTime to approach player, i.e. Larger value -> Slower
	int TotalNumOfCube;
	int AmbientLightBarNum;
	bool bIgnore;
	TArray<float> TimeCode;
	TArray<int> StyleCode;
	TArray<float> PosCode;
	TArray<FTimerHandle> AmbientEffectTimerCollection;

	// Create timers
	FTimerHandle PlayStartMainLoopTimer;
	FTimerHandle MusicTimer;
	FTimerHandle HealthRestoreTimer;
	FTimerHandle ShowCompletedTimer;
	FTimerHandle GameOverStateTimer;
	FTimerHandle RoundScoreTimer;
	FTimerHandle PlayerDieMenuTimer;
	FTimerHandle ReadyPlayerDieMenuTimer;
	FTimerHandle ReadyRoundScoreTimer;
	FTimerHandle ColorThemeTimer;
	FTimerHandle TrackLightsTimerOn;
	FTimerHandle TrackLightsTimerOff;
	FTimerHandle SpawnSpotLightAnimation;
	FTimerHandle AmbientLightBarTimer;
	FTimerHandle AmbientLightBarTimerSecondDelay;

	// Timer Functions
	void PlayStartMainLoop();

	// Delegate functions
	UFUNCTION()
	void PlayStart();

	// Functions
	void DecodeLevelCode();
	void SpawnCube();
	void HealthRestore();
	void GameStatePauseControl(bool bPause);
	void ShowSpawnSpotLight();
	void HideSpawnSpotLight();
	void ManageAmbientLightBars(int GenCount);
	void AmbientPulseEffect(int Mode, float Rate);
};