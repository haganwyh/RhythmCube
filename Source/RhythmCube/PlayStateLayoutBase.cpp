// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayStateLayoutBase.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "MainGameInstance.h"
#include "MainPlayerController.h"

void UPlayStateLayoutBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::Collapsed);

	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Bind delegates
	MainGameMode->OnReadyPlayState.AddDynamic(this, &UPlayStateLayoutBase::ReadyPlayState);
	MainGameMode->OnMainMenu.AddDynamic(this, &UPlayStateLayoutBase::MainMenu);
	MainGameMode->OnHealthChanged.AddDynamic(this, &UPlayStateLayoutBase::HealthChanged);
	MainGameMode->OnUnPauseState.AddDynamic(this, &UPlayStateLayoutBase::UnPauseState);

	// Set variables
	bFirstCreated = true;

	if (MainGameMode->bTutorial)
	{
		// Set difficulty display
		DifficultyText->SetText(FText::FromString(TEXT("Easy")));

		// Set song name text
		SongNameText->SetText(FText::FromString(TEXT("Tutorial")));
	}
	else
	{
		// Set difficulty display
		switch (MainGameInstance->SelectedDifficulty)
		{
		case 0:
			DifficultyText->SetText(FText::FromString(TEXT("Normal")));
			break;
		case 1:
			DifficultyText->SetText(FText::FromString(TEXT("Hard")));
			break;
		case 2:
			DifficultyText->SetText(FText::FromString(TEXT("Extreme")));
			break;
		default:;
		}

		// Set song name text
		SongNameText->SetText(FText::FromString(FString::Printf(TEXT("%s"), *MainGameMode->SongNames[MainGameInstance->SelectedSongID])));
	}

	// Wait for camera ready
	FTimerHandle AnimationTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this]
	{
		if (MainGameMode->bTutorial)
		{
			// Show keyboard layout
			MainGameMode->bEnablePause = true;
			MainPlayerController->HandlePause();
		}
		else
		{
			// READY! GO!
			PlayAnimation(ReadyGoInOut);
			PlayAnimation(FadeIn);
			if (MainGameInstance->bHardcoreMode)
			{
				PlayAnimation(HealthFadeIn);
			}
			bFirstCreated = false;
		}
	}, 2.f, false);
	
}

void UPlayStateLayoutBase::UpdateUI(float Score, int Combo)
{
	ScoreText->SetText(FText::AsNumber(static_cast<int>(roundf(Score))));
	ComboText->SetText(FText::AsNumber(Combo));
	// Set specific combo banner when reaching a milestone
	switch (Combo)
	{
	case 50:
		ShowComboBanner(0, TwoDigitSize);
		break;
	case 100:
		ShowComboBanner(1, ThreeDigitSize);
		break;
	case 200:
		ShowComboBanner(2, ThreeDigitSize);
		break;
	case 400:
		ShowComboBanner(3, ThreeDigitSize);
		break;
	case 600:
		ShowComboBanner(4, ThreeDigitSize);
		break;
	case 800:
		ShowComboBanner(5, ThreeDigitSize);
		break;
	case 1000:
		ShowComboBanner(6, FourDigitSize);
		break;
	case 1200:
		ShowComboBanner(7, FourDigitSize);
		break;
	case 1400:
		ShowComboBanner(8, FourDigitSize);
		break;
	case 1600:
		ShowComboBanner(9, FourDigitSize);
		break;
	case 1800:
		ShowComboBanner(10, FourDigitSize);
		break;
	case 2000:
		ShowComboBanner(11, FourDigitSize);
		break;
	default:;
	}
}

void UPlayStateLayoutBase::ShowComboBanner(int ID, FVector2D Size)
{
	// Set image and size
	ComboImage->SetBrushFromTexture(ComboImagesBase[ID]);
	ComboImage->SetDesiredSizeOverride(Size);
	
	// Sound effects
	Cast<AMainGameMode>(UGameplayStatics::GetGameMode(this))->PlaySoundEffects(SoundEffects::Boom);

	// Animation: In + Out
	PlayAnimation(ComboInOut);
}

void UPlayStateLayoutBase::ShowCompletedBanner()
{
	PlayAnimation(SongCompletedInOut);
}

void UPlayStateLayoutBase::ReadyPlayState(bool bNew)
{
	if (!bNew)
	{
		FTimerHandle AnimationTimer;
		GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this] {
			PlayAnimation(ReadyGoInOut);
			}, 1.f, false);
	}
}

void UPlayStateLayoutBase::MainMenu(bool bNew)
{
	PlayAnimation(FadeOut);
}

void UPlayStateLayoutBase::HealthChanged()
{
	if (HealthBar->GetPercent() > MainGameMode->Health / 100.f)
	{
		// Hurt
		PlayAnimation(HealthHurt);
		if (MainGameMode->Health < 30 && !bHealthWarn)
		{
			bHealthWarn = true;
			HealthWarn();
			GetWorld()->GetTimerManager().SetTimer(HealthWarnTimer, this, &UPlayStateLayoutBase::HealthWarn, 0.5f, true);
		}
	}
	HealthBar->SetPercent(MainGameMode->Health / 100.f);
}

void UPlayStateLayoutBase::HealthWarn()
{
	// Loop playing health warn animation until health back to normal
	if (HealthBar->GetPercent() < 0.3f)
	{
		PlayAnimation(HealthWarnInOut);
	}
	else
	{
		bHealthWarn = false;
		GetWorld()->GetTimerManager().ClearTimer(HealthWarnTimer);
	}
}

void UPlayStateLayoutBase::UnPauseState()
{
	if (bFirstCreated && MainGameMode->bTutorial)
	{
		bFirstCreated = false;

		FTimerHandle AnimationTimer;
		GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this]
		{
			PlayAnimation(FadeIn);
		}, 1.f, false);

		FTimerHandle StartGameTimer;
		GetWorld()->GetTimerManager().SetTimer(StartGameTimer, [this]
		{
			MainGameMode->OnPlayStart.Broadcast();
		}, 2.f, false);
	}
}