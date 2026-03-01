// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "Components/AudioComponent.h"
#include "CubeBase.h"
#include "TrackLightsBase.h"
#include "MainHUD.h"
#include "MainPlayerController.h"
#include "MainGameInstance.h"
#include "PlayStateLayoutBase.h"
#include "PlayPawn.h"
#include "UnrealClient.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SpotLightComponent.h"
#include "EngineUtils.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "TutorialLayoutBase.h"
#include "AmbientLightBarBase.h"

//// Initialise ////
void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Force load materials
	for (int i = 0; i < MaterialsToForceLoad.Num(); i++)
	{
		MaterialsToForceLoad[i]->SetForceMipLevelsToBeResident(true, true, 20.f);
	}

	// Set references
	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MainHUD = Cast<AMainHUD>(MainPlayerController->GetHUD());
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameUserSetting = UGameUserSettings::GetGameUserSettings();

	// Bind delegates
	OnPlayStart.AddDynamic(this, &AMainGameMode::PlayStart);

	TArray<AActor*> SpawnSpotLights;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnSpotLight"), SpawnSpotLights);
	SpawnSpotLight = SpawnSpotLights[0]->FindComponentByClass<USpotLightComponent>();

	// Start game program in MainHUD class
}

//// Game states ////

// Start state
void AMainGameMode::StartState()
{
	PlayBGM(BGM::StartingMenu, 0.f, 0.f);
	OnStartState.Broadcast();
}

void AMainGameMode::ReadyMainMenu()
{
	StopBGM(1.f);
	OnReadyMainMenu.Broadcast();
	FTimerHandle MenuUITimer;
	GetWorldTimerManager().SetTimer(MenuUITimer, [this] {
		MainMenu(true);
		// Update color theme
		UpdateColorThemeWithAnimation();
		}, 3.f, false);
}

// Main menu
void AMainGameMode::MainMenu(bool bNew)
{
	if (MainGameInstance->bNewPlayer)
	{
		// Tutorial for new players
		bTutorial = true;
	}

	OnMainMenu.Broadcast(bNew);
	if (!MainGameInstance->bNewPlayer)
	{
		if (bNew)
		{
			UpdateSelectedSong();
		}
		else
		{
			// Clear all running things
			StopBGM(1.f);
			GetWorldTimerManager().ClearTimer(PlayStartMainLoopTimer);
			GetWorldTimerManager().ClearTimer(HealthRestoreTimer);
			GetWorldTimerManager().ClearTimer(ShowCompletedTimer);
			GetWorldTimerManager().ClearTimer(GameOverStateTimer);
			GetWorldTimerManager().ClearTimer(RoundScoreTimer);
			FTimerHandle MenuSongTimer;
			GetWorldTimerManager().SetTimer(MenuSongTimer, [this] {
				UpdateSelectedSong();
				ShowSpawnSpotLight();
				}, 2.f, false);

			// Set back to using color theme
			UpdateColorTheme();
		}
	}
}

// Ready play state
void AMainGameMode::ReadyPlayState(bool bNew)
{
	HideSpawnSpotLight();

	// Stop game over state BGM
	StopBGM(1.f);

	// Initialise variables
	GeneratedCount = 0;
	TutorialCount = 0;
	Combo = 0;
	MaxCombo = 0;
	MoneyGain = 0;
	Score = 0;
	Health = 100;
	HitSuccessCount = 0;
	bManagedPlayerDieEvent = false;
	bHighestScore = false;
	GeneratedCubes = { {}, {}, {}, {} };
	GameStatePauseControl(false);
	DecodeLevelCode();
	LevelConstant = (TotalNumOfCube / 2.f) * (2.f * TotalNumOfCube + (TotalNumOfCube - 1.f) * -1.f); // Arithmetic series with constant difference -1
	OnReadyPlayState.Broadcast(bNew);
	OnHealthChanged.Broadcast();
	if (bTutorial)
	{
		UpdateColorTheme(Theme::BlueRed); // Tutorial is set up in the theme BLUE x RED
	}
	else
	{
		// Ambient light bars effect
		FTimerHandle AmbientLightBarTimerFirstDelay;
		GetWorldTimerManager().SetTimer(AmbientLightBarTimerFirstDelay, [this] {
			AmbientLightBarNum = 3;
			GetWorldTimerManager().SetTimer(AmbientLightBarTimer, [this] {
				if (AmbientLightBarNum >= 1) {
					AmbientLightBars[AmbientLightBarNum]->LightBarOn(AmbientLightBarNum * 0.5f);
					AmbientLightBarNum--;
				}
				else {
					GetWorldTimerManager().ClearTimer(AmbientLightBarTimer);
				}
			}, 0.5f, true);
		}, (bNew) ? 1.f : 0.01f, false);
		GetWorldTimerManager().SetTimer(AmbientLightBarTimerSecondDelay, [this] {
			AmbientPulseEffect(0, 0.05f);
		}, (bNew) ? 3.f : 2.f, false);

		// Wait for READY! GO! banner animation finished
		FTimerHandle PlayStartTimer;
		GetWorldTimerManager().SetTimer(PlayStartTimer, [this] {
			OnPlayStart.Broadcast();
			}, (bNew) ? 3.f : 2.f, false);
	}
}

// Play state
void AMainGameMode::PlayStart()
{
	// Play BGM with pre delay
	float MusicDelayTime = PreDelay * BpmTime * 4.f + BpmTime * 4.f * ((PreBeat - 4.f) / 4.f);
	if (bTutorial)
	{
		PlayBGM(BGM::SongTutorial, 0.f, 0.f);
		NextBeatTime = GetGameTimeSinceCreation() + abs(MusicDelayTime);
		NextTutorialTime = GetGameTimeSinceCreation();
	}
	else
	{
		if (MusicDelayTime > 0.f)
		{
			GetWorldTimerManager().SetTimer(MusicTimer, [this]
				{
					PlayBGM(static_cast<BGM>(MainGameInstance->SelectedSongID), 0.f, 0.f);
				}, MusicDelayTime, false);

			NextBeatTime = GetGameTimeSinceCreation();
		}
		else
		{
			PlayBGM(static_cast<BGM>(MainGameInstance->SelectedSongID), 0.f, 0.f);
			NextBeatTime = GetGameTimeSinceCreation() + abs(MusicDelayTime);
		}
	}
	
	// Main game loop, loop each frame
	GetWorldTimerManager().SetTimer(PlayStartMainLoopTimer, this, &AMainGameMode::PlayStartMainLoop, 0.01f, true);
	if (MainGameInstance->bHardcoreMode)
	{
		GetWorldTimerManager().SetTimer(HealthRestoreTimer, this, &AMainGameMode::HealthRestore, 1.f, true);
	}
	FTimerHandle EnablePauseTimer;
	GetWorldTimerManager().SetTimer(EnablePauseTimer, [this] {
		GameStatePauseControl(true);
		}, 1.f, false);
}

void AMainGameMode::PlayStartMainLoop()
{
	if (GetGameTimeSinceCreation() > NextBeatTime)
	{
		// Keep spawning cubes
		if (GeneratedCount >= TimeCode.Num())
		{
			// Reach the end of the file, enter game over state.
			GetWorldTimerManager().ClearTimer(PlayStartMainLoopTimer);

			GetWorldTimerManager().SetTimer(ShowCompletedTimer, [this]
				{
					FTimerHandle ShowCompletedTimerInner;
					GetWorldTimerManager().SetTimer(ShowCompletedTimer, [this]
						{
							MainHUD->PlayStateLayoutWidget->ShowCompletedBanner();

							// Ambient light bar
							AmbientPulseEffect(0, 0.05f);

						}, (Score == 1000000.f) ? 2.f : 0.01f, false); // Only wait 2 more seconds when it is full score
				}, 4 * BpmTime + 3.f + BpmTime * 4 * ((PreBeat - 4.f) / 4), false); // 3 seconds delay

			// Entering game over state
			GetWorldTimerManager().SetTimer(GameOverStateTimer, [this]
				{
					FTimerHandle GameOverStateTimerInner;
					GetWorldTimerManager().SetTimer(GameOverStateTimerInner, this, &AMainGameMode::GameOverState
						, (Score == 1000000.f) ? 2.f : 0.01f, false); // Only wait 2 more seconds when it is full score
				}, 4 * BpmTime + 6.f + BpmTime * 4 * ((PreBeat - 4) / 4), false);

			GetWorldTimerManager().SetTimer(RoundScoreTimer, [this]
				{
					GameStatePauseControl(false);

					// TECHNIQUE NAME: NO BUG PAUSE
					GetWorldTimerManager().SetTimer(ReadyRoundScoreTimer, [this] {
						// Ensure if players get full score, 1,000,000 should be shown rather than 999,999
						Score = roundf(Score);

						// Play special BGM if players get full score
						if (Score == 1000000.f)
						{
							PlayBGM(BGM::GameOverFullScore, 1.f, 5.f);
						}
						}, 0.1f, false);

					FTimerHandle NoBugPauseTimer;
					GetWorldTimerManager().SetTimer(NoBugPauseTimer, [this]
					{
						if (IsPaused())
						{
							GetWorldTimerManager().PauseTimer(ReadyRoundScoreTimer);
						}
					}, 0.05f, false);
				}, 4 * BpmTime + 2.f + BpmTime * 4 * ((PreBeat - 4.f) / 4), false); // 2 second delay
		}
		else
		{
			if (StyleCode[GeneratedCount] == -1) {
				// Delay node
				NextBeatTime += FMath::Abs(TimeCode[GeneratedCount] * BpmTime);
			}
			else if (StyleCode[GeneratedCount] <= 1) {
				// Spawn cubes
				SpawnCube();
				NextBeatTime += FMath::Abs(TimeCode[GeneratedCount] * BpmTime);
			}
			else if (StyleCode[GeneratedCount] == 2) {
				// Ambient light bars
				int TempGeneratedCount = GeneratedCount;
				FTimerHandle AmbientEffectTimer;
				GetWorldTimerManager().SetTimer(AmbientEffectTimer, [this, TempGeneratedCount, AmbientEffectTimer]
				{
					if (PosCode[TempGeneratedCount] <= 7) {
						ManageAmbientLightBars(TempGeneratedCount);
					}
					else if (PosCode[TempGeneratedCount] == 8) {
						AmbientPulseEffect(0, FMath::Abs(TimeCode[TempGeneratedCount]));
					}
					else if (PosCode[TempGeneratedCount] == 9) {
						AmbientPulseEffect(1, FMath::Abs(TimeCode[TempGeneratedCount]));
					}
					AmbientEffectTimerCollection.Remove(AmbientEffectTimer);
				}, PreBeat* BpmTime, false);
				AmbientEffectTimerCollection.Add(AmbientEffectTimer);

				if (TimeCode[GeneratedCount] > 0) {
					NextBeatTime += FMath::Abs(TimeCode[GeneratedCount] * BpmTime);
				}
			}
			GeneratedCount++;
		}
	}
	if (bTutorial && GetGameTimeSinceCreation() + 0.25f > NextTutorialTime) // 0.25 second is for the animation to come out
	{
		if (TutorialCount == TutorialTimeCode.Num())
		{
			GameStatePauseControl(false);
			MainHUD->TutorialLayoutWidget->EndTutorialText();
			NextTutorialTime = FLT_MAX; // Set to max so no more tutorial text will come out
		}
		else
		{
			MainHUD->TutorialLayoutWidget->UpdateTutorialText();
			NextTutorialTime += TutorialTimeCode[TutorialCount] * BpmTime;
			TutorialCount++;
		}
	}
}

// Game over state
void AMainGameMode::GameOverState()
{
	Score = roundf(Score); // Make sure score is rounded
	// Update variables required for game over state
	if (Combo > MaxCombo)
	{
		// Update max combo
		MaxCombo = Combo;
	}

	if (bTutorial)
	{
		// Only give extra money to players who first time play tutorial
		if (MainGameInstance->bNewPlayer)
		{
			// Calculate money gain
			MoneyGain = 2000.f;
			MainGameInstance->Money += MoneyGain;

			UE_LOG(LogTemp, Warning, TEXT("Grade: %s"), *UEnum::GetValueAsString(PlayerGrade));
			UE_LOG(LogTemp, Warning, TEXT("MoneyGain: %d"), MoneyGain);
		}

	}
	else
	{
		// Calculate money gain
		MoneyGain = Score / 1500 * (1 + 0.5 * MainGameInstance->SelectedDifficulty + 0.25 * MainGameInstance->bHardcoreMode);
		MainGameInstance->Money += MoneyGain;

		UE_LOG(LogTemp, Warning, TEXT("Grade: %s"), *UEnum::GetValueAsString(PlayerGrade));
		UE_LOG(LogTemp, Warning, TEXT("MoneyGain: %d"), MoneyGain);

		// Assign highest score
		int* HighestScore = MainGameInstance->GetHighestScore();
		if (HighestScore == nullptr)
		{
			MainGameInstance->AddHighestScore(Score);
			bHighestScore = true;
		}
		else
		{
			if (Score > *HighestScore)
			{
				// Update highest score
				*HighestScore = static_cast<int>(Score);
				bHighestScore = true;
			}
		}
	}

	// Calculate player grade
	PlayerGrade = GetPlayerGrade(Score);

	// Play BGM
	if (Score < 1000000)
	{
		// Play normal BGM if not full score
		PlayBGM(BGM::GameOverState, 0.f, 0.f);
	}

	// Remove new player label
	MainGameInstance->bNewPlayer = false;

	// Save game
	MainGameInstance->SaveGame();

	// Enter game over state
	OnGameOverState.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("Game over state"));
}

// Pause state
void AMainGameMode::PauseState()
{
	PauseBGM();
	GetWorldTimerManager().PauseTimer(MusicTimer); // This will cause slightly latency (Around 0.05 seconds)
	GetWorldTimerManager().PauseTimer(HealthRestoreTimer);
	GetWorldTimerManager().PauseTimer(ShowCompletedTimer);
	GetWorldTimerManager().PauseTimer(GameOverStateTimer);
	GetWorldTimerManager().PauseTimer(RoundScoreTimer);
	GetWorldTimerManager().PauseTimer(PlayStartMainLoopTimer);
	GetWorldTimerManager().PauseTimer(AmbientLightBarTimer);
	for (FTimerHandle Timer : AmbientEffectTimerCollection) {
		GetWorldTimerManager().PauseTimer(Timer);
	}
	OnPauseState.Broadcast();
}

// UnPause state
void AMainGameMode::UnPauseState()
{
	ResumeBGM();
	GetWorldTimerManager().UnPauseTimer(MusicTimer); // This will cause slightly latency (Around 0.05 seconds)
	GetWorldTimerManager().UnPauseTimer(HealthRestoreTimer);
	GetWorldTimerManager().UnPauseTimer(ShowCompletedTimer);
	GetWorldTimerManager().UnPauseTimer(GameOverStateTimer);
	GetWorldTimerManager().UnPauseTimer(RoundScoreTimer);
	GetWorldTimerManager().UnPauseTimer(ReadyPlayerDieMenuTimer); // Resume the player die main program
	GetWorldTimerManager().UnPauseTimer(ReadyRoundScoreTimer); // Resume the round score main program
	GetWorldTimerManager().UnPauseTimer(PlayStartMainLoopTimer);
	GetWorldTimerManager().UnPauseTimer(AmbientLightBarTimer);
	for (FTimerHandle Timer : AmbientEffectTimerCollection) {
		GetWorldTimerManager().UnPauseTimer(Timer);
	}
	OnUnPauseState.Broadcast();
}

// Setting state
void AMainGameMode::SettingState()
{
	OnSettingState.Broadcast();
	PlayBGM(BGM::SettingState, 0.5f, 2.f);
}

// Store state
void AMainGameMode::StoreState()
{
	OnStoreState.Broadcast();
	PlayBGM(BGM::StoreState, 0.5f, 2.f);
}

// Exit state
void AMainGameMode::ExitState()
{
	StopBGM(2.f);
	OnExitState.Broadcast();
	FTimerHandle EndSoundTimer;
	GetWorldTimerManager().SetTimer(EndSoundTimer, [this] {
		PlaySoundEffects(SoundEffects::SeeYouSoon);
		}, 1.f, false);
}

//// Functions ////
void AMainGameMode::DecodeLevelCode()
{
	// Get level code file
	FString File = FPaths::ProjectContentDir();
	if (bTutorial)
	{
		File.Append(FString::Printf(TEXT("LevelCodes/TutorialLevelCode.txt")));
	}
	else
	{
		File.Append(FString::Printf(TEXT("LevelCodes/%dLevelCode%d.txt"), MainGameInstance->SelectedSongID, MainGameInstance->SelectedDifficulty));
	}
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();
	FString FileContent;
	// Validate does the file exists
	if (FileManager.FileExists(*File))
	{
		if (FFileHelper::LoadFileToString(FileContent, *File, FFileHelper::EHashOptions::None))
		{
			// Initialise variables
			FString GroupedData = "";
			int Slot = 0;
			int GroupedCount = 0;
			TotalNumOfCube = 0;
			bIgnore = false;
			TimeCode.Empty();
			StyleCode.Empty();
			PosCode.Empty();
			// Start decode file
			for (int i = 0; i < FileContent.Len(); i++)
			{
				if (FileContent[i] == '/' && !bIgnore)
				{
					// Encounter '/', so split data into arrays
					if (GroupedCount >= 3)
					{
						// Level code decode
						switch (Slot)
						{
						case 0:
							TimeCode.Add(FCString::Atof(*GroupedData));
							break;
						case 1:
							StyleCode.Add(FCString::Atoi(*GroupedData));
							if (FCString::Atoi(*GroupedData) == 0 || FCString::Atoi(*GroupedData) == 1) TotalNumOfCube++;
							break;
						case 2:
							PosCode.Add(FCString::Atof(*GroupedData));
							break;
						default:;
						}
						Slot = (++Slot) % 3;
					}
					else
					{
						// Meta data decode (Bpm and speed data)
						switch (GroupedCount)
						{
						case 0:
							Bpm = FCString::Atof(*GroupedData);
							BpmTime = 60.f / Bpm; // Time between 1 beat
							break;
						case 1:
							Speed = FCString::Atoi(*GroupedData);
							break;
						case 2:
							PreDelay = FCString::Atof(*GroupedData);
							break;
						default:;
						}
					}
					GroupedCount++;
					GroupedData = "";
				}
				else if (FileContent[i] == '#')
				{
					// Decode mode -> Ignore mode | Ignore mode -> Decode mode
					bIgnore = bIgnore ? false : true;
				}
				else if (FileContent[i] != '\n' && !bIgnore)
				{
					// Grouping data between '/'
					GroupedData += FileContent[i];
				}
			}
		}
	}
	// PreBeat: The number of beat the cube spawn before the actual hitting beat come
	PreBeat = (Bpm > 100.f) ? 8 : 4;
	// Set up cube generation starting position
	CubeSpawnX = -30500.f * (PreBeat / 4) * (4.f / Speed); // 4 is a basic standard speed

	// For test
	UE_LOG(LogTemp, Warning, TEXT("Bpm: %f"), Bpm);
	UE_LOG(LogTemp, Warning, TEXT("Speed: %d"), Speed);
	for (int i = 0; i < TimeCode.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Time code: %f"), TimeCode[i]);
	}
	for (int i = 0; i < StyleCode.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Style code: %d"), StyleCode[i]);
	}
	for (int i = 0; i < PosCode.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pos code: %d"), PosCode[i]);
	}
}

void AMainGameMode::PlayBGM(BGM Song, float FadeOut, float FadeIn)
{
	StopBGM(FadeOut);
	// Create Sound2D object to play the BGM
	UAudioComponent* AudioToAdd = UGameplayStatics::SpawnSound2D(this, Audios[static_cast<int32>(Song)]);
	AudioComponents.Add(AudioToAdd);
	AudioComponents.Last()->Play();
	AudioComponents.Last()->FadeIn(FadeIn, 1.f);
}

void AMainGameMode::StopBGM(float FadeOut)
{
	while (AudioComponents.Num() > 0)
	{
		// Fade out and remove the audio components if it is playing, loop for all playing BGM
		if (AudioComponents[0] != nullptr)
		{
			if (!AudioComponents[0]->bIsPaused)
			{
				AudioComponents[0]->FadeOut(FadeOut, 0.f);
			}
		}
		AudioComponents.RemoveAt(0);
	}
}

void AMainGameMode::PauseBGM()
{
	// Validate there is actually a background music playing
	if (AudioComponents.Num() > 0)
	{
		if (AudioComponents[0] != nullptr)
		{
			AudioComponents[0]->SetPaused(true);
		}
		else
		{
			AudioComponents.RemoveAt(0);
		}
	}
}

void AMainGameMode::ResumeBGM()
{
	// Validate there is actually a background music playing
	if (AudioComponents.Num() > 0)
	{
		if (AudioComponents[0] != nullptr)
		{
			AudioComponents[0]->SetPaused(false);
		}
		else
		{
			AudioComponents.RemoveAt(0);
		}
	}
}

void AMainGameMode::PlaySoundEffects(SoundEffects Audio)
{
	// Create Sound2D object to play the sound effect
	UAudioComponent* SoundEffectsComponent = UGameplayStatics::SpawnSound2D(this, SoundEffectAudios[static_cast<int32>(Audio)]);
	SoundEffectsComponent->Play();
}

void AMainGameMode::SpawnCube()
{
	// Borderline x = -3500 | Spawn x = -30500 | Difference 27000
	// 0: y = 0, z = 200 | 1: y = -500, z = 500 | 2: y = 0, z = 800 | 3: y = 500, z = 500
	// Set spawn position
	// Cubes spawn 8 beats before music start
	FVector SpawnPosition;
	switch ((int)PosCode[GeneratedCount])
	{
	case 0:
		// Down
		SpawnPosition = FVector(CubeSpawnX, 0.f, 200.f);
		break;
	case 1:
		// Left
		SpawnPosition = FVector(CubeSpawnX, 500.f, 500.f);
		break;
	case 2:
		// Up
		SpawnPosition = FVector(CubeSpawnX, 0.f, 800.f);
		break;
	case 3:
		// Right
		SpawnPosition = FVector(CubeSpawnX, -500.f, 500.f);
		break;
	default:;
	}
	// Spawn cube
	// StyleCode[GeneratedCount] = 0 -> blue cube, = 1 -> red cube
	ACubeBase* CubeSpawned = Cast<ACubeBase>(GetWorld()->SpawnActor<AActor>(ActorsToSpawn[StyleCode[GeneratedCount]], SpawnPosition, FRotator(0.f, 0.f, 0.f)));
	// Register cube in array
	GeneratedCubes[PosCode[GeneratedCount]].Add(CubeSpawned);
	// Assign ID to spawned cube
	CubeSpawned->CubeID = StyleCode[GeneratedCount] * 4 + PosCode[GeneratedCount];
	CubeSpawned->ApplyInitialSettings();
	// Create delegate for spawned cube
	MainPlayerController->OnHitCube.AddDynamic(CubeSpawned, &ACubeBase::OnHitCube);
}

void AMainGameMode::DeregisterCube(ACubeBase* Target, int ID)
{
	// Deregister after 0.1 second, avoid remove all the cube at once
	FTimerHandle DeregisterTimer;
	GetWorldTimerManager().SetTimer(DeregisterTimer, [this, Target, ID]()
		{
			GeneratedCubes[ID % 4].Remove(Target);
		}, 0.01f, false);
}

void AMainGameMode::ResetCombo()
{
	if (Combo > MaxCombo)
	{
		// Update max combo
		MaxCombo = Combo;
	}
	if (Combo > 0)
	{
		PlaySoundEffects(SoundEffects::Miss);
	}
	Combo = 0;

	// Update widget
	MainHUD->PlayStateLayoutWidget->UpdateUI(Score, Combo);
}

void AMainGameMode::GainScore(Accuracy HitAccuracy)
{
	Combo++;
	if (HitAccuracy == Accuracy::Perfect)
	{
		Score += ((980000.f / TotalNumOfCube) + Combo * (20000.f / LevelConstant)); // Adjust the two floats can change the affect on score by combo count
	}
	else
	{
		Score += (((980000.f / TotalNumOfCube) + Combo * (20000.f / LevelConstant))) * 0.65; // 65% of original score if only get GOOD
	}

	// Update widget
	MainHUD->PlayStateLayoutWidget->UpdateUI(Score, Combo);
}

void AMainGameMode::UpdateSelectedSong()
{
	OnUpdateSelectedSong.Broadcast(MainGameInstance->SelectedSongID, SongNames[MainGameInstance->SelectedSongID]);
	PlayBGM(static_cast<BGM>(SongNames.Num() + MainGameInstance->SelectedSongID), 1.f, 0.f);
	MainGameInstance->SaveGame();
}

void AMainGameMode::Hurt()
{
	// Hurt only available in hardcore mode
	if (MainGameInstance->bHardcoreMode)
	{
		Health -= 10;
		OnHealthChanged.Broadcast();
		if (Health <= 0 && !bManagedPlayerDieEvent)
		{
			// Disable pause function
			/*
			=== IMPORTANT ===
			TECHNIQUE NAME: NO BUG PAUSE

			Here is a really good example of codes that make sure pause state won't overlap with other states,
			it basically delay all programs by 0.01 second (a frame) after disable the game state pause permission.

			The program will check after disabling the pause permission, that does the game is paused, this might caused
			by players press the pause button at the same time as the pause function is disabled.

			If it is paused, then the "ReadyPlayDieMenuTimer" will be paused, so all codes that to prepare for player die menu,
			will be paused. To continue running those codes, the timer will be resumed in the function "OnUnPaused()" in this class.
			*/
			bManagedPlayerDieEvent = true;
			GameStatePauseControl(false);
			MainPlayerController->SetInputMode(FInputModeUIOnly());
			GetWorldTimerManager().SetTimer(ReadyPlayerDieMenuTimer, [this] {
				UE_LOG(LogTemp, Warning, TEXT("DIe1"));
				// Clear all existed timers
				GetWorldTimerManager().ClearTimer(ShowCompletedTimer);
				GetWorldTimerManager().ClearTimer(GameOverStateTimer);
				GetWorldTimerManager().ClearTimer(PlayStartMainLoopTimer);
				GetWorldTimerManager().ClearTimer(HealthRestoreTimer);
				StopBGM(2.f);
				// Wait to show UI
				GetWorldTimerManager().SetTimer(PlayerDieMenuTimer, [this] {
					PlaySoundEffects(SoundEffects::Boom);
					OnPlayerDie.Broadcast();
					}, 4.f, false);
				}, 0.1f, false);

			// Pause timer if the game suddenly paused
			FTimerHandle NoBugPauseTimer;
			GetWorldTimerManager().SetTimer(NoBugPauseTimer, [this]
			{
				if (IsPaused())
				{
					GetWorldTimerManager().PauseTimer(ReadyPlayerDieMenuTimer);
				}
			}, 0.05f, false);
		}
	}
}

void AMainGameMode::HealthRestore()
{
	if (Health < 100)
	{
		Health++;
		OnHealthChanged.Broadcast();
	}
}

void AMainGameMode::GameStatePauseControl(bool bPause)
{
	bGameStateEnablePause = bPause;
	bEnablePause = bGameStateEnablePause;
}

void AMainGameMode::ShowSpawnSpotLight()
{
	GetWorldTimerManager().SetTimer(SpawnSpotLightAnimation, [this]
		{
			if (SpawnSpotLight->Intensity < 20000.f) {
				SpawnSpotLight->SetIntensity(SpawnSpotLight->Intensity + 20000.f / 60.f);
			}
			else {
				GetWorldTimerManager().ClearTimer(SpawnSpotLightAnimation);
			}
		}, 0.5f / 60.f, true);
}

void AMainGameMode::HideSpawnSpotLight()
{
	GetWorldTimerManager().SetTimer(SpawnSpotLightAnimation, [this]
		{
			if (SpawnSpotLight->Intensity > 0.f) {
				SpawnSpotLight->SetIntensity(SpawnSpotLight->Intensity - 10000.f / 60.f);
			}
			else {
				GetWorldTimerManager().ClearTimer(SpawnSpotLightAnimation);
			}
		}, 0.5f / 60.f, true);
}

void AMainGameMode::UpdateColorTheme()
{
	UpdateColorTheme(MainGameInstance->UsingTheme);
}

void AMainGameMode::UpdateColorTheme(Theme ColorTheme)
{
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ColorThemeParameterCollection,
		FName("LeftLineColor"), ThemeColors[static_cast<int32>(ColorTheme)][0]);
	UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ColorThemeParameterCollection,
		FName("RightLineColor"), ThemeColors[static_cast<int32>(ColorTheme)][1]);
	// Set color for left spotlights
	TrackLights[0]->UpdateColor(ThemeColors[static_cast<int32>(ColorTheme)][0]);
	// Set color for right spotlights
	TrackLights[1]->UpdateColor(ThemeColors[static_cast<int32>(ColorTheme)][1]);
}

void AMainGameMode::UpdateColorThemeWithAnimation()
{
	// Animation version of update color theme function
	LightsOpacity = 0.f;
	GetWorldTimerManager().SetTimer(ColorThemeTimer, [this] {
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ColorThemeParameterCollection,
				FName("LeftLineColor"), ThemeColors[static_cast<int32>(MainGameInstance->UsingTheme)][0] * LightsOpacity);
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ColorThemeParameterCollection,
				FName("RightLineColor"), ThemeColors[static_cast<int32>(MainGameInstance->UsingTheme)][1] * LightsOpacity);
			UKismetMaterialLibrary::SetVectorParameterValue(GetWorld(), ColorThemeParameterCollection,
				FName("LineFloorColor"), FLinearColor(1, 1, 1, 1) * LightsOpacity); // White color

			// Set color for left spotlights
			TrackLights[0]->UpdateColor(ThemeColors[static_cast<int32>(MainGameInstance->UsingTheme)][0] * LightsOpacity);
			// Set color for right spotlights
			TrackLights[1]->UpdateColor(ThemeColors[static_cast<int32>(MainGameInstance->UsingTheme)][1] * LightsOpacity);
			if (LightsOpacity >= 1.f)
			{
				GetWorldTimerManager().ClearTimer(ColorThemeTimer);
				UpdateColorTheme();
				UE_LOG(LogTemp, Warning, TEXT("Light animation done!"));
			}
			else
			{
				if (LightsOpacity < 0.1f)
				{
					// Small increase at start
					LightsOpacity += 0.005f;
				}
				else
				{
					// Large increase after 1 second
					LightsOpacity += 0.05f;
				}
			}
		}, 0.05f, true);
}

Grade AMainGameMode::GetPlayerGrade(int CurrentScore)
{
	Grade CurrentPlayerGrade = Grade::SSS;
	TArray<int> GradeThresholds = { 560000, 600000, 640000, 680000, 720000, 760000, 800000, 840000, 880000, 920000, 960000, 1000000 };
	TArray<Grade> Grades = { Grade::FF, Grade::C, Grade::CC, Grade::CCC, Grade::B, Grade::BB, Grade::BBB, Grade::A, Grade::AA, Grade::AAA, Grade::S, Grade::SS, Grade::SSS };
	for (int i = 0; i < GradeThresholds.Num(); i++)
	{
		if (CurrentScore < GradeThresholds[i])
		{
			CurrentPlayerGrade = Grades[i];
			break;
		}
	}
	return CurrentPlayerGrade;
}

void AMainGameMode::UpdateGraphics()
{
	GameUserSetting->SetOverallScalabilityLevel(MainGameInstance->Graphics);
	if (MainGameInstance->Graphics == 1)
	{
		GameUserSetting->SetReflectionQuality(0);
	}
	if (MainGameInstance->Graphics < 2)
	{
		GameUserSetting->SetAntiAliasingQuality(2);
	}
	GameUserSetting->SetResolutionScaleNormalized(1.f);
	GameUserSetting->SetVSyncEnabled(true);
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Graphics: %d"), MainGameInstance->Graphics)));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("Reflection: %d"), GameUserSetting->GetReflectionQuality())));
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("AntiAliasing: %d"), GameUserSetting->GetAntiAliasingQuality())));
	GameUserSetting->ApplySettings(false);
}

void AMainGameMode::UpdateFPS()
{
	GameUserSetting->SetFrameRateLimit(MainGameInstance->MaxFPS);
	GameUserSetting->ApplySettings(false);
}

void AMainGameMode::UpdateBGMVolume()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GameSoundMix, BGMSoundClass, MainGameInstance->BGMVolume);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), GameSoundMix);
}

void AMainGameMode::UpdateSFXVolume()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GameSoundMix, SFXSoundClass, MainGameInstance->SFXVolume);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), GameSoundMix);
}

void AMainGameMode::UpdateUIVolume()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GameSoundMix, UISoundClass, MainGameInstance->UIVolume);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), GameSoundMix);
}

void AMainGameMode::UpdateHitsoundVolume()
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GameSoundMix, HitSoundClass, MainGameInstance->HitsoundVolume);
	UGameplayStatics::PushSoundMixModifier(GetWorld(), GameSoundMix);
}

void AMainGameMode::BenchmarkGraphicsInitialize()
{
	// Restore settings
	UpdateBGMVolume();
	UpdateSFXVolume();
	UpdateUIVolume();
	UpdateHitsoundVolume();
	UpdateGraphics();
	UpdateFPS();

	if (MainGameInstance->bNewPlayer)
	{
		// Auto graphics setting for new players
		GameUserSetting->RunHardwareBenchmark();
		float BenchmarkResult = GameUserSetting->GetLastGPUBenchmarkResult();
		if (BenchmarkResult >= 300.f)
		{
			MainGameInstance->Graphics = 3;
		}
		else if (BenchmarkResult >= 200.f)
		{
			MainGameInstance->Graphics = 2;
		}
		else if (BenchmarkResult >= 100.f)
		{
			MainGameInstance->Graphics = 1;
		}
		else
		{
			MainGameInstance->Graphics = 0;
		}
		UE_LOG(LogTemp, Warning, TEXT("%d"), MainGameInstance->Graphics);
		UpdateGraphics();
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("CPUScore: %f"), GameUserSetting->GetLastCPUBenchmarkResult())));
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, *(FString::Printf(TEXT("GPUScore: %f"), GameUserSetting->GetLastGPUBenchmarkResult())));
		MainGameInstance->SaveGame();
	}
	OnLoadGame.Broadcast();
}

void AMainGameMode::ShowTrackLights(int ID, int Mode) {
	TrackLights[ID]->EnhanceTrackLights(Mode);
}

void AMainGameMode::ManageAmbientLightBars(int GenCount) {
	AmbientLightBars[PosCode[GenCount]]->LightBarOn(TimeCode[GenCount] * BpmTime);
}

void AMainGameMode::AmbientPulseEffect(int Mode, float Rate) {
	AmbientLightBarNum = Mode ? 7 : 0;
	GetWorldTimerManager().SetTimer(AmbientLightBarTimer, [this, Mode, Rate] {
		if (Mode ? AmbientLightBarNum >= 0 : AmbientLightBarNum < 8) {
			AmbientLightBars[AmbientLightBarNum]->LightBarOn(Rate * 1.f);
			if (Mode) AmbientLightBarNum--; else AmbientLightBarNum++;
		}
		else {
			GetWorldTimerManager().ClearTimer(AmbientLightBarTimer);
		}
	}, Rate, true);
}