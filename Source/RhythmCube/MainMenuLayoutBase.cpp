// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLayoutBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "MainGameInstance.h"
#include "MainPlayerController.h"

void UMainMenuLayoutBase::NativeOnInitialized()
{
	// Set references
	MainGameMode = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	MainGameInstance = Cast<UMainGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MainPlayerController = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	// Bind delegates
	MainGameMode->OnUpdateSongInfo.AddDynamic(this, &UMainMenuLayoutBase::CheckSongDisable);
	MainGameMode->OnUpdateSelectedSong.AddDynamic(this, &UMainMenuLayoutBase::CheckSongDisable);

	// Bind button delegates
	PlayButton->OnHovered.AddDynamic(this, &UMainMenuLayoutBase::PlayButtonHovered);
	PlayButton->OnUnhovered.AddDynamic(this, &UMainMenuLayoutBase::PlayButtonUnHovered);
	PlayButton->OnClicked.AddDynamic(this, &UMainMenuLayoutBase::PlayButtonClicked);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenuLayoutBase::ExitButtonClicked);
	SettingButton->OnClicked.AddDynamic(this, &UMainMenuLayoutBase::SettingButtonClicked);
	StoreButton->OnClicked.AddDynamic(this, &UMainMenuLayoutBase::StoreButtonClicked);

	// Animation
	if (MainGameInstance->bNewPlayer)
	{
		PlayAnimationReverse(FadeOutLimited);
	}
	else
	{
		CheckSongDisable(MainGameInstance->SelectedSongID, MainGameMode->SongNames[MainGameInstance->SelectedSongID]);
		PlayAnimationReverse(FadeOut);
	}
}

void UMainMenuLayoutBase::PlayButtonHovered()
{
	PlayAnimation(PlayButtonHover);
}

void UMainMenuLayoutBase::PlayButtonUnHovered()
{
	PlayAnimation(PlayButtonUnHover);
}

void UMainMenuLayoutBase::PlayButtonClicked()
{
	if (!MainPlayerController->bStartingAnimation)
	{
		MainGameMode->ReadyPlayState(true);
		RemoveWithAnimation();
	}
}

void UMainMenuLayoutBase::ExitButtonClicked()
{
	if (!MainPlayerController->bStartingAnimation)
	{
		ExitButton->SetIsEnabled(false);
		MainGameMode->ExitState();
	}
}

void UMainMenuLayoutBase::SettingButtonClicked()
{
	if (!MainPlayerController->bStartingAnimation)
	{
		SettingButton->SetIsEnabled(false);
		MainGameMode->SettingState();
	}
}

void UMainMenuLayoutBase::StoreButtonClicked()
{
	if (!MainPlayerController->bStartingAnimation)
	{
		StoreButton->SetIsEnabled(false);
		MainGameMode->StoreState();
	}
}

void UMainMenuLayoutBase::RemoveWithAnimation()
{
	if (MainGameInstance->bNewPlayer)
	{
		PlayAnimation(FadeOutLimited);
	}
	else
	{
		PlayAnimation(FadeOut);
	}
	FTimerHandle AnimationTimer;
	GetWorld()->GetTimerManager().SetTimer(AnimationTimer, [this] {
		RemoveFromParent();
		}, 1.f, false);
}

void UMainMenuLayoutBase::CheckSongDisable(int SongID, FString SongName)
{
	if (MainGameMode->DisableSongsFullID.Contains(SongID * 3 + MainGameInstance->SelectedDifficulty))
	{
		WarnText->SetVisibility(ESlateVisibility::Visible);
		PlayButton->SetIsEnabled(false);
	}
	else
	{
		WarnText->SetVisibility(ESlateVisibility::Collapsed);
		PlayButton->SetIsEnabled(true);
	}
}