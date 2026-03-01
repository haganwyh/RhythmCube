// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PlayPawn.h"
#include "MainHUD.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "PlayStateLayoutBase.h"

//// Initialise ////
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// Bind delegates
	MainGameMode->OnReadyPlayState.AddDynamic(this, &AMainPlayerController::ReadyPlayState);
	MainGameMode->OnGameOverState.AddDynamic(this, &AMainPlayerController::GameOverState);
	MainGameMode->OnMainMenu.AddDynamic(this, &AMainPlayerController::MainMenu);
	MainGameMode->OnPlayerDie.AddDynamic(this, &AMainPlayerController::PlayerDie);
	MainGameMode->OnStartState.AddDynamic(this, &AMainPlayerController::StartState);
	MainGameMode->OnReadyMainMenu.AddDynamic(this, &AMainPlayerController::ReadyMainMenu);

	// Initialize variables
	bIsPaused = false;

	// Setup input subsystem
	InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	// Bind actions
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	BindAllActions();
}

//// Game states ////

// Start state
void AMainPlayerController::StartState()
{
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());
}

// Ready Main menu
void AMainPlayerController::ReadyMainMenu()
{
	bStartingAnimation = true;
	SetViewTargetWithBlend(MenuStateActor.LoadSynchronous(), 5.f, EViewTargetBlendFunction::VTBlend_EaseInOut, 4.5f);
	FTimerHandle PossessTimer;
	GetWorldTimerManager().SetTimer(PossessTimer, [this] {
		Possess(MenuStatePawn.LoadSynchronous());
		bStartingAnimation = false;
		}, 5.f, false);
}

// Main menu
void AMainPlayerController::MainMenu(bool bNew)
{
	bShowMouseCursor = true;
	SetInputMode(FInputModeGameAndUI());

	if (!bNew)
	{
		SetPause(false);
		bIsPaused = false;
		SetViewTargetWithBlend(MenuStateActor.LoadSynchronous(), 2.f, EViewTargetBlendFunction::VTBlend_EaseInOut, 5.f);
		FTimerHandle PossessTimer;
		GetWorldTimerManager().SetTimer(PossessTimer, [this] {
			Possess(MenuStatePawn.LoadSynchronous());
			}, 2.f, false);
	}
}

// Ready play state
void AMainPlayerController::ReadyPlayState(bool bNew)
{
	// Set camera position
	SetViewTargetWithBlend(PlayStateActor.LoadSynchronous(), 2.f, EViewTargetBlendFunction::VTBlend_EaseInOut, 5.f);
	FTimerHandle PossessTimer;
	GetWorldTimerManager().SetTimer(PossessTimer, [this] {
		Possess(PlayStatePawn.LoadSynchronous());
		}, 2.f, false);

	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

// Game over state
void AMainPlayerController::GameOverState()
{
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

//// Functions ////
void AMainPlayerController::BindAllActions()
{
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContent, 0);
	
	// Bind actions into trigger functions
	EnhancedInputComponent->BindAction(ActionBlueDown, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleBlueDown);
	EnhancedInputComponent->BindAction(ActionBlueLeft, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleBlueLeft);
	EnhancedInputComponent->BindAction(ActionBlueUp, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleBlueUp);
	EnhancedInputComponent->BindAction(ActionBlueRight, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleBlueRight);

	EnhancedInputComponent->BindAction(ActionRedDown, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleRedDown);
	EnhancedInputComponent->BindAction(ActionRedLeft, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleRedLeft);
	EnhancedInputComponent->BindAction(ActionRedUp, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleRedUp);
	EnhancedInputComponent->BindAction(ActionRedRight, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleRedRight);

	EnhancedInputComponent->BindAction(ActionPause, ETriggerEvent::Triggered, this, &AMainPlayerController::HandlePause);

	EnhancedInputComponent->BindAction(ActionQuitTutorial, ETriggerEvent::Triggered, this, &AMainPlayerController::HandleQuitTutorial);
}

// Broadcast for cubes with specific ID
// If key need to swap, bKeySwap will be 1, then 0 -> 4, 4 -> 0
void AMainPlayerController::HandleBlueDown()
{
	OnHitCube.Broadcast(0 + 4 * MainGameInstance->bKeySwap * !MainGameMode->bTutorial);
}
void AMainPlayerController::HandleBlueLeft()
{
	OnHitCube.Broadcast(1 + 4 * MainGameInstance->bKeySwap * !MainGameMode->bTutorial);
}
void AMainPlayerController::HandleBlueUp()
{
	OnHitCube.Broadcast(2 + 4 * MainGameInstance->bKeySwap * !MainGameMode->bTutorial);
}
void AMainPlayerController::HandleBlueRight()
{
	OnHitCube.Broadcast(3 + 4 * MainGameInstance->bKeySwap * !MainGameMode->bTutorial);
}
void AMainPlayerController::HandleRedDown()
{
	OnHitCube.Broadcast(4 - 4 * MainGameInstance->bKeySwap * !MainGameMode->bTutorial);
}
void AMainPlayerController::HandleRedLeft()
{
	OnHitCube.Broadcast(5 - 4 * MainGameInstance->bKeySwap * !MainGameMode->bTutorial);
}
void AMainPlayerController::HandleRedUp()
{
	OnHitCube.Broadcast(6 - 4 * MainGameInstance->bKeySwap * !MainGameMode->bTutorial);
}
void AMainPlayerController::HandleRedRight()
{
	OnHitCube.Broadcast(7 - 4 * MainGameInstance->bKeySwap * !MainGameMode->bTutorial);
}

// Pause game
void AMainPlayerController::HandlePause()
{
	if (MainGameMode->bEnablePause && !bIsPaused || bIsPaused)
	{
		bIsPaused = !bIsPaused;
		if (MainGameMode->bTutorial)
		{
			// Tutorial pause
			if (bIsPaused)
			{
				// Pause
				MainGameMode->bEnablePause = false; // Avoid players to pause the game again in pause state
				MainGameMode->PauseState();
				SetPause(bIsPaused);
				MainGameMode->PlaySoundEffects(SoundEffects::StartClick);
			}
			else
			{
				// UnPause
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
				MainGameMode->PlaySoundEffects(SoundEffects::Click);
			}
		}
		else
		{
			// Normal pause
			bShowMouseCursor = bIsPaused;
			if (bIsPaused)
			{
				// Pause
				MainGameMode->bEnablePause = false; // Avoid players to pause the game again in pause state
				SetInputMode(FInputModeGameAndUI());
				MainGameMode->PauseState();
				SetPause(bIsPaused);
			}
			else
			{
				// UnPause
				if (MainGameMode->bManagedPlayerDieEvent)
				{
					SetInputMode(FInputModeUIOnly());
				}
				else
				{
					SetInputMode(FInputModeGameOnly());
				}
				Cast<AMainHUD>(GetHUD())->UnPauseCountDown();
				// MainHUD class will carry on the resume function
			}
			MainGameMode->PlaySoundEffects(SoundEffects::Click);
		}
	}
}

void AMainPlayerController::HandleQuitTutorial()
{
	if (bIsPaused && MainGameMode->bTutorial && !MainGameInstance->bNewPlayer)
	{
		MainGameMode->GameOverState();
		MainGameMode->PlaySoundEffects(SoundEffects::Click);
		if (roundf(MainGameMode->Score) == 1000000.f)
		{
			MainGameMode->PlayBGM(BGM::GameOverState, 0.f, 0.f);
		}
	}
}

void AMainPlayerController::PlayerDie()
{
	bShowMouseCursor = true;
}