// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "PlayStateLayoutBase.h"
#include "MainPlayerController.h"
#include "GameOverStateLayoutBase.h"
#include "PauseStateLayoutBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PausedTimerManagerTicker.h"
#include "MainMenuLayoutBase.h"
#include "ExitStateLayoutBase.h"
#include "GameOverHardcoreLayoutBase.h"
#include "SettingStateLayoutBase.h"
#include "StoreStateLayoutBase.h"
#include "StartingMenuLayoutBase.h"
#include "PreStartingMenuLayoutBase.h"
#include "TutorialPauseStateLayoutBase.h"
#include "TutorialLayoutBase.h"

//// Initialize ////
void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind delegates
	MainGameMode->OnMainMenu.AddDynamic(this, &AMainHUD::MainMenu);
	MainGameMode->OnPlayStart.AddDynamic(this, &AMainHUD::PlayStart);
	MainGameMode->OnGameOverState.AddDynamic(this, &AMainHUD::GameOverState);
	MainGameMode->OnReadyPlayState.AddDynamic(this, &AMainHUD::ReadyPlayState);
	MainGameMode->OnPauseState.AddDynamic(this, &AMainHUD::PauseState);
	MainGameMode->OnExitState.AddDynamic(this, &AMainHUD::ExitState);
	MainGameMode->OnPlayerDie.AddDynamic(this, &AMainHUD::PlayerDie);
	MainGameMode->OnSettingState.AddDynamic(this, &AMainHUD::SettingState);
	MainGameMode->OnStoreState.AddDynamic(this, &AMainHUD::StoreState);
	MainGameMode->OnStartState.AddDynamic(this, &AMainHUD::StartState);

	// Pre starting page
	PreStartingMenuLayoutWidget = CreateWidget<UPreStartingMenuLayoutBase>(GetWorld(), PreStartingMenuLayoutClass);
	PreStartingMenuLayoutWidget->AddToViewport();
}

//// Game states ////

// Start state
void AMainHUD::StartState()
{
	// Add widgets
	StartingMenuLayoutWidget = CreateWidget<UStartingMenuLayoutBase>(GetWorld(), StartingMenuLayoutClass);
	StartingMenuLayoutWidget->AddToViewport();
}

// Main menu
void AMainHUD::MainMenu(bool bNew)
{
	if (!bNew)
	{
		// From home button
		// Remove pointers
		PauseStateLayoutWidget = nullptr;
		GameOverHardcoreLayoutWidget = nullptr;
		GameOverStateLayoutWidget = nullptr;
		TutorialPauseStateLayoutWidget = nullptr;

		FTimerHandle WidgetTimer;
		GetWorldTimerManager().SetTimer(WidgetTimer, [this] {
			// Show main menu after camera animation
			MainMenuLayoutWidget = CreateWidget<UMainMenuLayoutBase>(GetWorld(), MainMenuLayoutClass);
			MainMenuLayoutWidget->AddToViewport();

			// And then remove play state UI
			PlayStateLayoutWidget->RemoveFromParent();
			PlayStateLayoutWidget = nullptr;
			}, 2.f, false);
	}
	else
	{
		// Start from main menu
		// Remove pointers
		SettingStateLayoutWidget = nullptr;
		StoreStateLayoutWidget = nullptr;
		PreStartingMenuLayoutWidget = nullptr;
		StartingMenuLayoutWidget = nullptr;

		MainMenuLayoutWidget = CreateWidget<UMainMenuLayoutBase>(GetWorld(), MainMenuLayoutClass);
		MainMenuLayoutWidget->AddToViewport();
	}
}


// Ready play state
void AMainHUD::ReadyPlayState(bool bNew)
{
	if (bNew) // First load up?
	{
		// Remove pointers
		MainMenuLayoutWidget = nullptr;

		// Load UI and display
		if (MainGameMode->bTutorial)
		{
			TutorialLayoutWidget = CreateWidget<UTutorialLayoutBase>(GetWorld(), TutorialLayoutClass);
			TutorialLayoutWidget->AddToViewport();
		}

		PlayStateLayoutWidget = CreateWidget<UPlayStateLayoutBase>(GetWorld(), PlayStateLayoutClass);
		PlayStateLayoutWidget->AddToViewport();

		// Load up pause state first and hide, allow fast show up without loading
		if (!MainGameMode->bTutorial)
		{
			PauseStateLayoutWidget = CreateWidget<UPauseStateLayoutBase>(GetWorld(), PauseStateLayoutClass);
			PauseStateLayoutWidget->AddToViewport();
		}
	}
	else // From retry button
	{
		// Remove pointers
		GameOverStateLayoutWidget = nullptr;
		GameOverHardcoreLayoutWidget = nullptr;

		// Reset UI
		PlayStateLayoutWidget->UpdateUI(0.f, 0);
	}
}

// Play state
void AMainHUD::PlayStart()
{
	
}

// Game over state
void AMainHUD::GameOverState()
{
	// Remove tutorial UI
	if (TutorialLayoutWidget)
	{
		TutorialLayoutWidget->RemoveFromParent();
		TutorialLayoutWidget = nullptr;
	}


	// Load UI and display
	GameOverStateLayoutWidget = CreateWidget<UGameOverStateLayoutBase>(GetWorld(), GameOverStateLayoutClass);
	GameOverStateLayoutWidget->AddToViewport();
}

// Pause state
void AMainHUD::PauseState()
{
	// Show UI
	if (MainGameMode->bTutorial)
	{
		TutorialPauseStateLayoutWidget = CreateWidget<UTutorialPauseStateLayoutBase>(GetWorld(), TutorialPauseStateLayoutClass);
		TutorialPauseStateLayoutWidget->AddToViewport();
	}
	else
	{
		PauseStateLayoutWidget->PlayAnimation(PauseStateLayoutWidget->FadeIn);
	}
}

// UnPause state
void AMainHUD::UnPauseCountDown()
{
	// Hide UI
	PauseStateLayoutWidget->PlayAnimation(PauseStateLayoutWidget->FadeOut);
	// Resume game after 3.25 seconds, wait for 3,2,1 animation done
	FTimerHandle PauseTimer;
	GetWorldTimerManager().SetTimer(PauseTimer, [this]
	{
		MainGameMode->UnPauseState();
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetPause(false);
		FTimerHandle EnablePauseTimer;
		// Enable pause after 0.5 seconds
		GetWorldTimerManager().SetTimer(EnablePauseTimer, [this] 
		{
			if (MainGameMode->bGameStateEnablePause)
			{
				MainGameMode->bEnablePause = true;
			}
		}, 0.5f, false);
	}, 3.25f, false);
}

// Exit state
void AMainHUD::ExitState()
{
	ExitStateLayoutWidget = CreateWidget<UExitStateLayoutBase>(GetWorld(), ExitStateLayoutClass);
	ExitStateLayoutWidget->AddToViewport();
}

void AMainHUD::PlayerDie()
{
	GameOverHardcoreLayoutWidget = CreateWidget<UGameOverHardcoreLayoutBase>(GetWorld(), GameOverHardcoreLayoutClass);
	GameOverHardcoreLayoutWidget->AddToViewport();
}

// Setting state
void AMainHUD::SettingState()
{
	// Remove widgets
	MainMenuLayoutWidget->RemoveWithAnimation();
	MainMenuLayoutWidget = nullptr;
	// Add widgets
	SettingStateLayoutWidget = CreateWidget<USettingStateLayoutBase>(GetWorld(), SettingStateLayoutClass);
	SettingStateLayoutWidget->AddToViewport();
}

// Store state
void AMainHUD::StoreState()
{
	// Remove widgets
	MainMenuLayoutWidget->RemoveWithAnimation();
	MainMenuLayoutWidget = nullptr;
	// Add widgets
	StoreStateLayoutWidget = CreateWidget<UStoreStateLayoutBase>(GetWorld(), StoreStateLayoutClass);
	StoreStateLayoutWidget->AddToViewport();
}